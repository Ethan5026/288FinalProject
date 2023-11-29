#include "timer.h"
#include "calibrate.h" //Calibrate: calibrations for the specific cyBot and update constants
#include "uart.h"
#include "icecreamScan.h" //include ir and ping and pwm
#include "movement.h"
#include "open_interface.h"
#include "userDisplay.h" //include lcd and buttons
#include "entertainer.h" //fun happy ice cream music :)
#include "lcd.h"
#include<math.h>

int inPath(obj_t* obj);
void avoidObstacle(oi_t* oi, int turnDirection, int exitCode);

int DRIVE_STEP = 4;
int TURN_STEP = 4;

/**
 * main.c
 */
int main(void)
{
    timer_init();
    icecreamScan_init();
    lcd_init();
    lcd_printf("Press the CLEAN\nbutton on the rumba");
    oi_t oi;
    oi_init(&oi);
    lcd_clear();
    entertainer_init();
    //entertainer_play();
    move_init();

    lcd_init();
    button_init();

    obj_t objs[10];
    while (1) {
        icecreamScan_scan(objs, 0, 180);
        obj_t* obj = objs;

        int clearDistance = MAXIMUM_OBJECT_DISTANCE;
        while (obj->isObject) {
            lcd_printf("Angle: %d\nDist: %.2lf\nWidth: %.2lf\nPath: %d", obj->angle, obj->distance, obj->width, inPath(obj));
            timer_waitMillis(1000);
            if (inPath(obj) && obj->distance < clearDistance) {
                clearDistance = (int)obj->distance;
            }
            obj++;
        }
        lcd_printf("Driving: %d", clearDistance);
        timer_waitMillis(1000);

        switch (drive(&oi, clearDistance, 1).exitCode)  // Change to icecreamScan_drive
        {
        case 0: // Success
            break;
        case 1: // bump right
            avoidObstacle(&oi, -1, 1);
            break;
        case 2: // bump left
            avoidObstacle(&oi, 1, 2);
            break;
        case 3: //hole
            drive(&oi, -DRIVE_STEP, 0);
            turn(&oi, 90);
            break;
        case 4: //Customer found
            //userDisplay_sell();
            break;
        }
    }

    oi_free(&oi);
}

int inPath(obj_t* obj) {
    int ROOMBA_WIDTH = 35;
    int LEEWAY = 5;
    return fabs(obj->distance*cos(obj->angle * M_PI / 180)) - obj->width/2 - ROOMBA_WIDTH/2 - LEEWAY < 0;
}

void avoidObstacle(oi_t* oi, int turnDirection, int exitCode) {
    while (1) {
        drive(oi, -1, 0);
        turn(oi, turnDirection * TURN_STEP);
        if (drive(oi, DRIVE_STEP, 1).exitCode != exitCode) {
            break;
        }
    }
}
