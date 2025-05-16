#include "tm4c123gh6pm.h"
#include "fixing fp.h"
#include "GPIO.h"
#include "UART.h"
#include "LCD.h"
#include "Distance.h"
#include "My built-in Functions.h"
#include "LED.h"
#include "GPS.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define PI 3.14159f

int main(void)
{
    FPU_Enable(); // Enable FPU before using float math
    portc_init();
    GPIO_INIT();
    LCD_INIT();
    delay(10000);
    UART7_Init();
    while (1)
    {
        Extract_data_get_distance_display();
    }
}
