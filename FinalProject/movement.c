#include "open_interface.h"
#include "movement.h"

void move_init(){
    distance_coef = 1;
    angle_coef = 1.22;
}

void move_forward(oi_t *sensor, int centimeters){

    double sum = 0;
    oi_setWheels(250, 250);
    while(fabs(sum) < fabs(centimeters * 10.0 * distance_coef)){
        oi_update(sensor);
        sum += sensor->distance;
    }


    oi_setWheels(0, 0);
}

void move_dynamic(oi_t *sensor, int centimeters){


    double sum = 0;
    int sidewaysMvmt = 0;
    oi_setWheels(250, 250);
    while(fabs(sum) < fabs(centimeters * 10.0 * distance_coef)){
        oi_update(sensor);
        sum += sensor->distance;

        if(sensor->bumpLeft){
            oi_setWheels(-250, -250);
            double sum2 = 0;
            oi_setWheels(-250, -250);

            while(sum2 < 150){
                oi_update(sensor);
                sum2 -= sensor->distance;
            }

            turn_clockwise(sensor, 90);


            sum2 = 0;
            oi_setWheels(250,250);
            while(sum2 < 250){
            oi_update(sensor);
            sum2 += sensor->distance;
            }


            turn_counterclockwise(sensor,90);
            oi_setWheels(250,250);
            sidewaysMvmt += 150;
            sum -= 150;


        }
        else if(sensor->bumpRight){

            double sum2 = 0;
            oi_setWheels(-250, -250);
            while(sum2 < 150){
                oi_update(sensor);
                sum2 -= sensor->distance;
            }

            turn_counterclockwise(sensor, 90);
            oi_setWheels(250,250);

            sum2 = 0;
            while(sum2 < 250){
            oi_update(sensor);
            sum2 += sensor->distance;
            }


            turn_clockwise(sensor,90);
            oi_setWheels(250,250);
            sidewaysMvmt -= 150;
            sum -= 150;
        }
    }
    sum = 0;
    if(sidewaysMvmt < 0){turn_clockwise(sensor, 90);}
    else if (sidewaysMvmt > 0 ){turn_counterclockwise(sensor, 90);}
    move_forward(sensor, fabs(sidewaysMvmt) / 10);


    oi_setWheels(0, 0);

}
void move_backward(oi_t *sensor, int centimeters){


    double sum = 0;
    oi_setWheels(-250, -250);
    while(fabs(sum) < fabs(centimeters * 10.0 * distance_coef)){
        oi_update(sensor);
        sum -= sensor->distance;
    }

    oi_setWheels(0, 0);


}


void turn_clockwise(oi_t *sensor, int degrees){

    double sum = 0;
    oi_setWheels(-100, 100);
    while(fabs(sum) < fabs(degrees * angle_coef)){
        oi_update(sensor);
        sum -= sensor->angle;

    }
    oi_setWheels(0, 0);


}

void turn_counterclockwise(oi_t *sensor, int degrees){
    double sum = 0;
    oi_setWheels(100, -100);
    while(fabs(sum) < fabs(degrees * angle_coef)){
        oi_update(sensor);
        sum += sensor->angle;

    }
    oi_setWheels(0, 0);


}
void turn(oi_t *sensor, int degrees){

    if(degrees > 0){
        turn_counterclockwise(sensor, degrees);
    }
    else{
        turn_clockwise(sensor,-degrees);
    }
}

void move(oi_t *sensor, int centimeters) {
    if (centimeters > 0) {
        move_forward(sensor, centimeters);
    }
    else {
        move_backward(sensor, -centimeters);
    }
}
