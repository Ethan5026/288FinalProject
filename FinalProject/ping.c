#include "ping.h"
#include "lcd.h"
#include "Timer.h"

volatile enum {LOW, HIGH, DONE} state;
volatile unsigned int rising_time;
volatile unsigned int falling_time;
volatile unsigned int overflow;


void ping_init(){

    SYSCTL_RCGCGPIO_R |= 0x2; //assign clock for port B
    GPIO_PORTB_AFSEL_R |= 0x8; //sets peripheral signal for pin 3
    GPIO_PORTB_PCTL_R &= ~0x0000F000; //sets pin 3 for device 7
    GPIO_PORTB_PCTL_R |= 0x00007000; //sets pin 3 for device 7
    GPIO_PORTB_DIR_R &= ~0x8; // set as output
    GPIO_PORTB_DEN_R |= 0x8; // enable port

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3; // Turn on clock to TIMER3
    timer_waitMillis(10);

    TIMER3_CTL_R &= ~0x0100; //clear
    TIMER3_CFG_R = TIMER_CFG_16_BIT; //splits the timers to be 16 bit mode
    TIMER3_TBMR_R = 0x7; //edge time and capture mode
    TIMER3_TBILR_R = 0xFFFF; // set max timer
    TIMER3_TBPR_R |= 0xFF;

    TIMER3_IMR_R &= ~0xFFF; // clear interrupts
    TIMER3_IMR_R |= TIMER_IMR_CBEIM; // set interrupt
    NVIC_EN1_R |= 0x10; // enable interrupt 36
    TIMER3_ICR_R |= 0x400; //clear interrupt
    TIMER3_CTL_R |= 0x0C00; //enable both falls

    IntRegister(INT_TIMER3B, TIMER3B_Handler); //bind handler
    IntMasterEnable();
    TIMER3_CTL_R |= 0x0100; // enable the timer
    overflow = 0;
    ping_coef = .00108;
}
void TIMER3B_Handler(){
    int time = TIMER3_TBR_R;
    if(state == LOW){
        rising_time = time;
        state = HIGH;
    }
    else if(state == HIGH){
        falling_time = time;
        state = DONE;
    }
    TIMER3_ICR_R |= 0x400;
    timer_waitMicros(10);
    TIMER3_ICR_R &= ~0x400;

}

double ping_receive(){
    state = DONE;
    pulse();
    state = LOW;

    while(state != DONE){}

    //overflow detection
    if(rising_time < falling_time){
        overflow++;
        rising_time += 0xFFFFFF;
    }
    return (rising_time - falling_time) * ping_coef;

}

void pulse(){
    GPIO_PORTB_AFSEL_R &= ~0x8;
    GPIO_PORTB_DIR_R |= 0x8;// set as an output
    GPIO_PORTB_DATA_R |= 0x8; // set pb3 to high
    timer_waitMicros(5); //wait 5 microseconds
    GPIO_PORTB_DATA_R &= ~0x8; // set pb3 to low
    GPIO_PORTB_AFSEL_R |= 0x8;
    GPIO_PORTB_DIR_R &= ~0x8; //makes pin 3 an input


}
