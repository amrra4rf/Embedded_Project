#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "Distance.h"
#include "My built-in Functions.h"

#define PI 3.14159f
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    char name[6];

} data_locations;
////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert degrees to radians
float toRadians(float degrees)
{
    return degrees * (PI / 180.0f);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Convert from NMEA DDDMM.MMMM format to decimal degrees
float convertNMEAtoDecimal(const char *coord)
{
    float raw = my_strtof(coord); // safer than atof
    int degrees = (int)(raw / 100);
    float minutes = raw - (degrees * 100);
    return degrees + (minutes / 60.0f);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Haversine formula
double calculateDistance(float lat1, float lon1, float lat2, float lon2)
{
    float dLat = toRadians(lat2 - lat1);
    float dLon = toRadians(lon2 - lon1);
    lat1 = toRadians(lat1);
    lat2 = toRadians(lat2);

    float s1 = sinf(dLat / 2);
    float s2 = sinf(dLon / 2);
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
////////////////////////////////////////////////////////////////////////////////////////////////////
data_locations locations[5] =
    {
        {"Credit"},
        {"Hall C"},
        {"Hall A"},
        {"Lubann"},
        {"Mosque"}

};
////////////////////////////////////////////////////////////////////////////////////////////////////
double min_distance(float our_lat, float our_lon, const double place_lat[], const double place_lon[], char n[])
{
    float min_dist = 1000000000000000000.0;
    volatile float d;
    for (int i = 0; i < 5; i++)
    {
        d = calculateDistance(our_lat, our_lon, place_lat[i], place_lon[i]);
        if (d < min_dist)
        {
            min_dist = d;
            strcpy(n, locations[i].name);
        }
    }
    return min_dist;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
