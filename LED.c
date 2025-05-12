#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "LED.h"

////////////////////////////////////////////
void rgb_write(float dis)
{
    if (dis >= 100)
    {
        GPIO_PORTF_DATA_R |= 0X02; // red fire
    }
    else if (dis >= 50)
    {
        GPIO_PORTF_DATA_R |= 0X0A; // yellow fire
    }
    else
    {
        GPIO_PORTF_DATA_R |= 0X08; // green fire
    }
}
////////////////////////////////////////////
