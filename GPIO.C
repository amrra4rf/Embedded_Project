#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "GPIO.h"
//////////////////////////////////////////////////////////////////////////
//for buzzer
void portc_init()
{
	SYSCTL_RCGCGPIO_R |= 0x04;             // 1) activate clock for Port c
    while((SYSCTL_PRGPIO_R & 0x04) == 0);  // wait for stabilization

    GPIO_PORTC_LOCK_R = 0x4C4F434B;        // 2) unlock GPIO Port F
    GPIO_PORTC_CR_R = 0x1F;                // allow changes to PF4-0

    GPIO_PORTC_AMSEL_R = 0x00;             // 3) disable analog on PF
    GPIO_PORTC_PCTL_R = 0x00000000;        // 4) PCTL GPIO on PF4-0
    GPIO_PORTC_DIR_R = 0x10;               // 5) PF4,PF0 in, PF3-1 out
    GPIO_PORTC_AFSEL_R = 0x00;             // 6) disable alt funct on PF4-0
    GPIO_PORTC_DEN_R = 0x1F;               // 7) enable digital I/O on PF4-0
}
//////////////////////////////////////////////////////////////////////////
//for LED
void RGB_Init(void){ 
    SYSCTL_RCGCGPIO_R |= 0x20;            // Enable clock for PORTF
    while((SYSCTL_PRGPIO_R & 0x20)==0);   // Wait for clock to stabilize
    GPIO_PORTF_LOCK_R = 0x4C4F434B;       // Unlock PORTF
    GPIO_PORTF_CR_R |= 0x0E;              // Allow changes to PF1-3
    GPIO_PORTF_AMSEL_R &= ~0x0E;          // Disable analog on PF1-3
    GPIO_PORTF_PCTL_R &= ~0x0000FFF0;     
    GPIO_PORTF_AFSEL_R &= ~0x0E;          // Disable alt functions on PF1-3
    GPIO_PORTF_DIR_R |= 0x0E;             // Set PF1-3 as output
    GPIO_PORTF_DEN_R |= 0x0E;             // Enable digital I/O on PF1-3
    GPIO_PORTF_DATA_R &= ~0x0E;           // Turn off all LEDs initially
}
//////////////////////////////////////////////////////////////////////////
//for LCD
void GPIO_INIT(void){
		//enable clock of A,B,D,E
		SYSCTL_RCGCGPIO_R |= 0x1B; 
		//Digitalize Data
		GPIO_PORTA_DEN_R |=0xE0;
		GPIO_PORTB_DEN_R |=0x13;
		GPIO_PORTE_DEN_R |=0x30;
		GPIO_PORTD_DEN_R |=0x07;
		//Set Direction to output
		GPIO_PORTA_DIR_R |=0xE0;
		GPIO_PORTB_DIR_R |=0x13;
		GPIO_PORTE_DIR_R |=0x30;
		GPIO_PORTD_DIR_R |=0x07;
		// Clear Alternate Functions
    GPIO_PORTA_AFSEL_R &= ~0xE0;
    GPIO_PORTB_AFSEL_R &= ~0x13;
    GPIO_PORTD_AFSEL_R &= ~0x07;
    GPIO_PORTE_AFSEL_R &= ~0x30;
}
//////////////////////////////////////////////////////////////////////////
