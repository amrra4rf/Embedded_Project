#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <math.h>
#include <string.h>

#define PI 3.14159f

float convertNMEAtoDecimal(const char *coord);
double calculateDistance(float lat1, float lon1, float lat2, float lon2);
double min_distance( float our_lat,float our_lon,const double place_lat[],const double place_lon[],char n[]);
 