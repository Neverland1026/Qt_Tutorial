#ifndef GPS_H
#define GPS_H

#include "exif.h"
#include <iostream>
#include <vector>

using namespace std;

class GPS
{
public:
    GPS();

    double getLat();
    double getLon();
    double getAlt();

    void setInfo(double lat, double lon, double alt);
    bool cmpGPS(double lonR = 180.0000,
                double lonL = 0.0000,
                double latT = 90.0000,
                double latB = 0.0000);

    int readGPSFromPhoto(const string &str);

    friend ostream & operator<<(ostream &os,const GPS &gps);

    string getPara(int index);

private:
    double _latitude; //经度
    double _longitude; //纬度
    double _altitude; //高程
    string _path;

    vector<string> _para;
};

#endif // GPS_H
