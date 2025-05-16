#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "UART.h"
//////////////////////////////////////////////////////////////////////
//for receiving GPS data
void UART7_Init(void){
    SYSCTL_RCGCUART_R |= 0x80;     // Enable UART7
    SYSCTL_RCGCGPIO_R |= 0x10;     // Enable GPIOE
    while((SYSCTL_PRGPIO_R & 0x10) == 0) {};

    UART7_CTL_R &= ~UART_CTL_UARTEN;
    UART7_IBRD_R = 104;            // 9600 baud
    UART7_FBRD_R = 11;
    UART7_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN;
    UART7_CC_R = UART_CC_CS_SYSCLK;
    UART7_CTL_R |= UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN;

    GPIO_PORTE_AFSEL_R |= 0x03;
    GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & 0xFFFFFF00) | 0x00000011;
    GPIO_PORTE_DEN_R |= 0x03;
    GPIO_PORTE_AMSEL_R &= ~0x03;
}
//////////////////////////////////////////////////////////////////////
//for reading GPS data
char UART7_ReadChar(void){
    while((UART7_FR_R & UART_FR_RXFE) != 0);
    return (char)(UART7_DR_R & 0xFF);
}
//////////////////////////////////////////////////////////////////////