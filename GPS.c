#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <string.h>
#include "GPIO.h"
#include "UART.h"
#include "Distance.h"
#include "LCD.h"
#include "My built-in Functions.h"
#include "LED.h"
#include "GPS.h"

///////////////////////////////////////////////////////////////////////////////////////////
void Extract_data_get_distance_display(void)
{
    char buffer[100];
    int idx;
    char c;
    char latStr[20] = {0}, lonStr[20] = {0};
    char latDir = 'N', lonDir = 'E';
    float lat, lon;
    char nnn[6];
    const double latitudes[] = {30.0635127, 30.063778, 30.064140, 30.0634105, 30.064685};
    const double longitudes[] = {31.2784245, 31.280511, 31.280202, 31.2795464, 31.280378};
    c = UART7_ReadChar();
    if (c == '$')
    {
        idx = 0;
        buffer[idx++] = c;

        while (1)
        {
            c = UART7_ReadChar();
            if (c == '\r')
                continue;
            if (c == '\n')
                break;
            if (idx < 99)
                buffer[idx++] = c;
        }
        buffer[idx] = '\0';

        if (strncmp(buffer + 1, "GPRMC", 5) == 0)
        {
            // Extract fields
            char *token = strtok(buffer, ",");
            int field = 0;

            while (token != NULL)
            {
                field++;

                if (field == 4)
                    strncpy(latStr, token, sizeof(latStr) - 1);
                else if (field == 5)
                    latDir = token[0];
                else if (field == 6)
                    strncpy(lonStr, token, sizeof(lonStr) - 1);
                else if (field == 7)
                    lonDir = token[0];

                token = strtok(NULL, ",");
            }
            // Convert NMEA to decimal degrees
            lat = convertNMEAtoDecimal(latStr);
            lon = convertNMEAtoDecimal(lonStr);

            // Apply direction
            if (latDir == 'S')
                lat = -lat;
            if (lonDir == 'W')
                lon = -lon;
            // Calculate distance
            volatile float temp_distance2 = min_distance(lat, lon, latitudes, longitudes, nnn) * 1000;
            LCD_cmd(0x80); // cursor at fisrt row first column
            char output[20];
            float_to_string(temp_distance2, output, 2); // Converts to "42.67"
            LCD_string(nnn, 6);

            LCD_cmd(0xC0);
            LCD_string(output, sizeof(output) - 16);
            RGB_Init();
            rgb_write(temp_distance2);

            if (temp_distance2 <= 15)
            {
                GPIO_PORTC_DATA_R |= 0x10;
                delay(1000000);
                GPIO_PORTC_DATA_R &= 0x00;
            }
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////