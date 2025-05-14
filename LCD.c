#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "LCD.h"
///////////////////////////////////////////////////////////////////////////////////
// delay loop
void delay(long delay)
{
    while (delay--)
        ;
}
///////////////////////////////////////////////////////////////////////////////////
// RS=PD0		RW=PD1		EN=PD2
// D0=PA7		D1=PA6		D2=PA5		D3=PB4		D4=PE5		D5=PE4		D6=PB1		D7=PB0
void print_data(unsigned char data)
{
    if ((data & 0x01) == 0x01)
    {
        GPIO_PORTA_DATA_R |= (1 << 7);
    }
    else
    {
        GPIO_PORTA_DATA_R &= ~(1 << 7);
    }
    if ((data & 0x02) == 0x02)
    {
        GPIO_PORTA_DATA_R |= (1 << 6);
    }
    else
    {
        GPIO_PORTA_DATA_R &= ~(1 << 6);
    }
    if ((data & 0x04) == 0x04)
    {
        GPIO_PORTA_DATA_R |= (1 << 5);
    }
    else
    {
        GPIO_PORTA_DATA_R &= ~(1 << 5);
    }
    if ((data & 0x08) == 0x08)
    {
        GPIO_PORTB_DATA_R |= (1 << 4);
    }
    else
    {
        GPIO_PORTB_DATA_R &= ~(1 << 4);
    }
    if ((data & 0x10) == 0x10)
    {
        GPIO_PORTE_DATA_R |= (1 << 5);
    }
    else
    {
        GPIO_PORTE_DATA_R &= ~(1 << 5);
    }
    if ((data & 0x20) == 0x20)
    {
        GPIO_PORTE_DATA_R |= (1 << 4);
    }
    else
    {
        GPIO_PORTE_DATA_R &= ~(1 << 4);
    }
    if ((data & 0x40) == 0x40)
    {
        GPIO_PORTB_DATA_R |= (1 << 1);
    }
    else
    {
        GPIO_PORTB_DATA_R &= ~(1 << 1);
    }
    if ((data & 0x80) == 0x80)
    {
        GPIO_PORTB_DATA_R |= (1 << 0);
    }
    else
    {
        GPIO_PORTB_DATA_R &= ~(1 << 0);
    }
}
///////////////////////////////////////////////////////////////////////////////////
void LCD_data(unsigned char data)
{
    print_data(data);            // pass 8bit data to data lines
    GPIO_PORTD_DATA_R &= ~(0X2); // turn off R/W for write operation in LCD
    GPIO_PORTD_DATA_R |= (0X1);  // turn on RS for writing to data register of lcd
    GPIO_PORTD_DATA_R |= (0X4);  // turn on EN for enabling clock of lcd
    delay(10000);                // wait for sometime
    GPIO_PORTD_DATA_R &= ~(0X4); // turn off EN for enabling clock of lcd
}
///////////////////////////////////////////////////////////////////////////////////
void LCD_cmd(unsigned char cmd)
{
    print_data(cmd);             // pass 8bit data to data lines
    GPIO_PORTD_DATA_R &= ~(0X2); // turn off R/W for write operation in LCD
    GPIO_PORTD_DATA_R &= ~(0X1); // turn off RS for writing to instruction register of lcd
    GPIO_PORTD_DATA_R |= (0X4);  // turn on EN for enabling clock of lcd
    delay(10000);                // wait for sometime
    GPIO_PORTD_DATA_R &= ~(0X4); // turn off EN for enabling clock of lcd
}
///////////////////////////////////////////////////////////////////////////////////
void LCD_string(char *str, unsigned char len)
{
    char i;
    for (i = 0; i < len; i++)
    {
        LCD_data(str[i]);
    }
}
///////////////////////////////////////////////////////////////////////////////////
void LCD_INIT(void)
{
    LCD_cmd(0x38); // 8bit mode 16columns 2rows
    LCD_cmd(0x06); // auto increment curser when printing current data
    LCD_cmd(0x0C); // cursor off and display on
    LCD_cmd(0x01); // clear screen
    delay(10000);
}
///////////////////////////////////////////////////////////////////////////////////
