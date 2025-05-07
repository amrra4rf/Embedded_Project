#include "tm4c123gh6pm.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define PI 3.14159f
#define EARTH_RADIUS 6371000  // in meters
//begin of sttruct
 typedef struct  
{
	char name[6];
	
}data_locations;

//end of struct
//RX UART7 ====PE0
// Enable FPU
void FPU_Enable(void) {
    // Set bits 20-23 in CPACR to enable CP10 and CP11 (floating point)
    *((volatile unsigned long *)0xE000ED88) |= 0x00F00000;
}
void delay (long delay){
		while(delay--);
}
// Convert degrees to radians
float toRadians(float degrees) {
    return degrees * (PI / 180.0f);
}
// mustrtofloat
float my_strtof(const char *str) {
    float result = 0.0f;
    float frac = 0.0f;
    int sign = 1;
    int i = 0;
    int frac_div = 1;
    int after_decimal = 0;

    // Skip whitespace
    while (str[i] == ' ' || str[i] == '\t') i++;

    // Handle sign
    if (str[i] == '-') {
        sign = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }

    // Process digits
    while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
        if (str[i] == '.') {
            after_decimal = 1;
            i++;
            continue;
        }

        int digit = str[i] - '0';
        if (!after_decimal) {
            result = result * 10.0f + digit;
        } else {
            frac = frac * 10.0f + digit;
            frac_div *= 10;
        }
        i++;
    }

    result += frac / frac_div;
    return result * sign;
}

// Convert from NMEA DDDMM.MMMM format to decimal degrees
float convertNMEAtoDecimal(const char *coord) {
    float raw = my_strtof(coord);  // safer than atof
    int degrees = (int)(raw / 100);
    float minutes = raw - (degrees * 100);
    return degrees + (minutes / 60.0f);
}

// Haversine formula
double calculateDistance(float lat1, float lon1, float lat2, float lon2) {
    float dLat = toRadians(lat2 - lat1);
    float dLon = toRadians(lon2 - lon1);
    lat1 = toRadians(lat1);
    lat2 = toRadians(lat2);


		float s1 = sinf(dLat/2);
		float s2 = sinf(dLon/2);
		float c1 = cosf(lat1);
		float c2 = cosf(lat2);

		float a = s1 * s1 + c1 * c2 * s2 * s2;
		float sqrtA = sqrtf(a);
		float sqrtOneMinusA = sqrtf(1.0f - a);
		float angle = atan2f(sqrtA, sqrtOneMinusA);
		float c = 2.0f * angle;
		volatile float a_dbg = a;
		volatile float c_dbg = c;
		volatile float result = c * 6371.0;
		return result; 
}
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

char UART7_ReadChar(void){
    while((UART7_FR_R & UART_FR_RXFE) != 0);
    return (char)(UART7_DR_R & 0xFF);
}/*
void UART0_Init(void){            
    SYSCTL_RCGCUART_R |= 0x01;     // Enable UART0
    SYSCTL_RCGCGPIO_R |= 0x01;     // Enable GPIOA
    while((SYSCTL_PRGPIO_R & 0x01) == 0) {};

    UART0_CTL_R &= ~UART_CTL_UARTEN;
    UART0_IBRD_R = 104;            // 9600 baud
    UART0_FBRD_R = 11;
    UART0_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN;
    UART0_CC_R = UART_CC_CS_SYSCLK;
    UART0_CTL_R |= UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN;

    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x00000011;
    GPIO_PORTA_DEN_R |= 0x03;
    GPIO_PORTA_AMSEL_R &= ~0x03;
}
void UART0_WriteChar(char c){
    while((UART0_FR_R & UART_FR_TXFF) != 0);
    UART0_DR_R = c;
}

void UART0_WriteString(char* str){
    while(*str){
        UART0_WriteChar(*str++);
    }
}*/
//RS=PD0		RW=PD1		EN=PD2		
//D0=PA7		D1=PA6		D2=PA5		D3=PB4		D4=PE5		D5=PE4		D6=PB1		D7=PB0
void print_data(unsigned char data){
		if((data&0x01)==0x01){GPIO_PORTA_DATA_R|=(1<<7);}
		else {GPIO_PORTA_DATA_R&=~(1<<7);}
		if((data&0x02)==0x02){GPIO_PORTA_DATA_R|=(1<<6);}
		else {GPIO_PORTA_DATA_R&=~(1<<6);}
		if((data&0x04)==0x04){GPIO_PORTA_DATA_R|=(1<<5);}
		else {GPIO_PORTA_DATA_R&=~(1<<5);}
		if((data&0x08)==0x08){GPIO_PORTB_DATA_R|=(1<<4);}
		else {GPIO_PORTB_DATA_R&=~(1<<4);}
		if((data&0x10)==0x10){GPIO_PORTE_DATA_R|=(1<<5);}
		else {GPIO_PORTE_DATA_R&=~(1<<5);}
		if((data&0x20)==0x20){GPIO_PORTE_DATA_R|=(1<<4);}
		else {GPIO_PORTE_DATA_R&=~(1<<4);}
		if((data&0x40)==0x40){GPIO_PORTB_DATA_R|=(1<<1);}
		else {GPIO_PORTB_DATA_R&=~(1<<1);}
		if((data&0x80)==0x80){GPIO_PORTB_DATA_R|=(1<<0);}
		else {GPIO_PORTB_DATA_R&=~(1<<0);}			
}

void LCD_data(unsigned char data){
		print_data(data); 						//pass 8bit data to data lines
		GPIO_PORTD_DATA_R &= ~(0X2);  //turn off R/W for write operation in LCD
		GPIO_PORTD_DATA_R |=  (0X1);	//turn on RS for writing to data register of lcd
		GPIO_PORTD_DATA_R |=  (0X4);	//turn on EN for enabling clock of lcd
		delay(10000);									//wait for sometime
		GPIO_PORTD_DATA_R &= ~(0X4);	//turn off EN for enabling clock of lcd	
}

void LCD_cmd(unsigned char cmd){
		print_data(cmd); 						//pass 8bit data to data lines
		GPIO_PORTD_DATA_R &= ~(0X2);  //turn off R/W for write operation in LCD
		GPIO_PORTD_DATA_R &= ~(0X1);	//turn off RS for writing to instruction register of lcd
		GPIO_PORTD_DATA_R |=  (0X4);	//turn on EN for enabling clock of lcd
		delay(10000);									//wait for sometime
		GPIO_PORTD_DATA_R &= ~(0X4);	//turn off EN for enabling clock of lcd	
}

void LCD_string( char *str,unsigned char len){
		char i;
		for(i=0;i<len;i++){
			LCD_data(str[i]);
		}
}

void LCD_INIT(void){
		LCD_cmd(0x38);		//8bit mode 16columns 2rows
		LCD_cmd(0x06);		//auto increment curser when printing current data
		LCD_cmd(0x0C);		//cursor off and display on
		LCD_cmd(0x01);		//clear screen
		delay(10000);
}
void GPIO_INIT(void){
		SYSCTL_RCGCGPIO_R |= 0x1B; //enable clock of A,B,D,E
/*		while(SYSCTL_RCGCGPIO_R &= 0x1B==0);
		GPIO_PORTA_LOCK_R = 0x4C4F434B;
		GPIO_PORTB_LOCK_R = 0x4C4F434B;
		GPIO_PORTD_LOCK_R = 0x4C4F434B;
		GPIO_PORTE_LOCK_R = 0x4C4F434B;
		GPIO_PORTA_CR_R = 0x1F;
		GPIO_PORTB_CR_R = 0x1F;
		GPIO_PORTD_CR_R = 0x1F;
		GPIO_PORTE_CR_R = 0x1F;*/
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
void float_to_string(float num, char *str, int precision) {
    if (num < 0) {
        *str++ = '-';
        num = -num;
    }

    int int_part = (int)num;
    float frac_part = num - int_part;

    // Convert integer part to string
    char temp[10];
    int i = 0;
    if (int_part == 0) {
        temp[i++] = '0';
    } else {
        while (int_part > 0) {
            temp[i++] = '0' + (int_part % 10);
            int_part /= 10;
        }
    }

    // Reverse and copy
    while (i > 0) {
        *str++ = temp[--i];
    }

    *str++ = '.'; // Decimal point

    // Convert fractional part
    for (int j = 0; j < precision; j++) {
        frac_part *= 10;
        int digit = (int)frac_part;
        *str++ = '0' + digit;
        frac_part -= digit;
    }

    *str = '\0'; // Null-terminate
}


data_locations locations [5]=
	{
		{"Credit"},
		{"Hall C"},
		{"Hall A"},
		{"Luban"},
		{"Mosque"}
			
	};
 double min_distance( float our_lat,float our_lon,const double place_lat[],const double place_lon[],char n[]){
		float min_dist=1000000000000000000.0;
	 volatile float d;
		for(int i=0;i<5;i++){
			 d=calculateDistance(our_lat,our_lon,place_lat[i],place_lon[i]);
			if(d<min_dist){
					min_dist=d;
		      strcpy(n,locations[i].name);
				
			}
			
		}
 return min_dist;
 }
 
 
  


const double latitudes[]  = {30.063394,30.063778 ,30.064140,30.063189,30.064685};
const double longitudes[] = {31.278293, 31.280511, 31.280202,31.279429,31.280378};
int main(void) {
	
	
		
	
		//char y[6];
		//strcpy(y,locations[0].name);
    FPU_Enable();  // Enable FPU before using float math
    //char sentence[] = "$GPRMC,163522.00,A,3004.30318,N,03122.20801,E,0.287,,010525,,,A*78";
		char buffer[100];
    int idx;
    char c;
		GPIO_INIT();
		LCD_INIT();
		delay(10000);
    // Variables
    char latStr[20] = {0}, lonStr[20] = {0};
    char latDir = 'N', lonDir = 'E';
    float lat, lon;
    //float refLat = 30.063778f;
    //float refLon = 31.280511f;
    //UART0_Init();
    UART7_Init();
		while(1){
        c = UART7_ReadChar();
        if(c == '$'){
            idx = 0;
            buffer[idx++] = c;

								while(1){
                c = UART7_ReadChar();
                if(c == '\r') continue;
                if(c == '\n') break;
                if(idx < 99)
                    buffer[idx++] = c;
							}
            buffer[idx] = '\0';

            if(strncmp(buffer + 1, "GPRMC", 5) == 0){
                //UART0_WriteString("Received GPRMC:\r\n");
                //UART0_WriteString(buffer);
                //UART0_WriteChar('\r');
                //UART0_WriteChar('\n');
		// Extract fields
    char *token = strtok(buffer, ",");
    int field = 0;

    while (token != NULL) {
        field++;

        if (field == 4) strncpy(latStr, token, sizeof(latStr) - 1);
        else if (field == 5) latDir = token[0];
        else if (field == 6) strncpy(lonStr, token, sizeof(lonStr) - 1);
        else if (field == 7) lonDir = token[0];

        token = strtok(NULL, ",");
    }
    // Convert NMEA to decimal degrees
    lat = convertNMEAtoDecimal(latStr);
    lon = convertNMEAtoDecimal(lonStr);

    // Apply direction
    if (latDir == 'S') lat = -lat;
    if (lonDir == 'W') lon = -lon;

    // Calculate distance
		char nnn[6];

   // volatile double mindistance = min_distance(locations ,lat, lon,nnn);//*1000 for meter
			//volatile float temp_distance=calculateDistance(lat,lon,30.063394f,31.278293f);
			//float temp_lat=latitudes[0];
			//float temp_lon=longitudes[0];
			volatile float temp_distance2=min_distance(lat,lon,latitudes,longitudes,nnn)*1000;
		LCD_cmd(0x80);		// cursor at fisrt row first column
		char output[20];
		float_to_string(temp_distance2, output, 2); // Converts to "42.67"
		LCD_string(nnn,6);
		
		LCD_cmd(0xC0);
		LCD_string(output,sizeof(output)-16);
		//LCD_string(y,sizeof(y));
						}
					}
	}
}



    /*sprintf(msg, "Lat token: %s\r\n", lat);
    UART0_WriteString(msg);
    sprintf(msg, "Lat dir: %c\r\n", lat_dir);
    UART0_WriteString(msg);
    sprintf(msg, "Lon token: %s\r\n", lon);
    UART0_WriteString(msg);
    sprintf(msg, "Lon dir: %c\r\n", lon_dir);
    UART0_WriteString(msg);*/