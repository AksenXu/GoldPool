#include <stdio.h>
#include <math.h>
	   
#define     EARTH_RADIUS        6378137  //m
#define     PI                  3.1415926

static double rad(double d) //deg2rad
{
  return d * PI / 180.0;
}

struct gpsinfo
{
	double latitude;
	double longitude;
};

int main() 
{
	gpsinfo lastpos = {23.542879, 113.929671};
	gpsinfo nowpos = {23.542879, 113.929671};
	//gpsinfo nowpos =  {22.543299, 114.926437};
	
    double radLat1 = rad(lastpos.latitude);
    double radLat2 = rad(nowpos.latitude);
    double radLong1 = rad(lastpos.longitude);
    double radLong2 = rad(nowpos.longitude);
    
    double a = radLat1 - radLat2;
    double b = radLong1 - radLong2;
    double s = 2 * asin(sqrt(pow(sin(a/2),2) + cos(radLat1)*cos(radLat2)*pow(sin(b/2),2)));
    s = s * EARTH_RADIUS;
    s = round(s * 10000) / 10000;

    int distance = (int)s;
    printf("calcDistance %f,%f => %f,%f distance %dm\n", lastpos.latitude, lastpos.longitude, nowpos.latitude, nowpos.longitude, distance);
    
	return 0;
}