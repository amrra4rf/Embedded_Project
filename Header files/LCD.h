#include "tm4c123gh6pm.h"
#include <stdint.h>
void delay (long delay);
void LCD_data(unsigned char data);
void LCD_cmd(unsigned char cmd);
void LCD_string( char *str,unsigned char len);
void LCD_INIT(void);
