/*
*
*   uart.c
*
*
*
*
*
*   @author
*   @date
*/

#include "uart.h"
#include "timer.h"


void uart_init(void){
    SYSCTL_RCGCGPIO_R |= 0b00000010;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0b00000010;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R |= 0b00000011;
    GPIO_PORTB_PCTL_R  &= 0xFFFFFF00; // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  |= 0x00000011;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R   |= 0b00000011;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R   &= 0b11111110;        // sets pb0 as input
    GPIO_PORTB_DIR_R   |= 0b00000010;       // sets pb1 as output

    //compute baud values [UART clock= 16 MHz]
    UART1_CTL_R &= 0xFFFFFFFE;    // disable UART1 (page 918)
    UART1_IBRD_R = 8;//(int)ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = 44;//(int)((fbrd - ibrd) * 64 + 0.5);   // write fractional portion of BRD to FBRD
    UART1_LCRH_R |= 0b01100000;        // write serial communication parameters (page 916) * 8bit and no parity
    UART1_CC_R   &= 0x0;          // use system clock as clock source (page 939)
    UART1_CTL_R |= 0b0001;        // enable UART1 and smart clock
    uart_flag = 0; //data has not been read
}

void uart_sendChar(char data){

	while(UART1_FR_R & 0b100000){
	}
	UART1_DR_R = data;
}

char uart_receive(void){
	while(UART1_FR_R & 0b10000){

	}
	return (char)(UART1_DR_R & 0xFF);
}

void uart_sendStr(const char *data){
    int i = 0;
	while(1){
	    char c = data[i];
	    i++;
	    if(c == '\0'){
	        break;
	    }
	    uart_sendChar(c);
	}
}
int uart_hasNextByte(void){
    return UART1_FR_R & 0b10000;
}

void uart1_handler(){
    uart_data = uart_receive();
    uart_sendChar(29);
    uart_sendChar(uart_data);
    uart_sendChar('\n');
    uart_flag = 1;
    UART1_ICR_R |= 0x10; // clear the interrupt
    timer_waitMicros(100); // wait for the interrupt to clear
    UART1_ICR_R &= ~0x10; //stop clearing interrupt
}
void uart_handlerInit(){
    UART1_IM_R |= 0x10;// enable receive interrupt
    NVIC_EN0_R |= 0x40; // set the NVIC
   IntRegister(INT_UART1, uart1_handler); // bind the interrupt to the handler
}
