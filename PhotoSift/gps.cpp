#include "gps.h"
#include <windows.h>
#include <sstream>
#include <iomanip>

GPS::GPS()
{

}

double GPS::getLat(){return _latitude;}
double GPS::getLon(){return _longitude;}
double GPS::getAlt(){return _altitude;}

void GPS::setInfo(double lat, double lon, double alt){
    _latitude = lat;
    _longitude = lon;
    _altitude = alt;
}

ostream & operator<<(ostream &os,const GPS& gps){
    os<<gps._path<<"  "<<gps._latitude<<"  "
     <<gps._longitude<<"  "
    <<gps._altitude<<endl;
    return os;
}

bool GPS::cmpGPS(double lonR, double lonL, double latT, double latB){
    if(_latitude <latT && _latitude >latB && _longitude < lonR && _longitude>lonL)
        return true;
    return false;
}

double &round(double &src,int bits){
    stringstream ss;
    ss<<fixed<<setprecision(bits)<<src;
    ss>>src;
    return src;
}

string GPS::getPara(int index){
    return _para[index];
}

int GPS::readGPSFromPhoto(const string &str){

    // eg: "E:\\DSC04094.JPG"; //双反斜杠

    // Read the JPEG file into a buffer
    string s = str;
    const char * photoPath = s.data();
    FILE *fp = fopen(photoPath, "rb");
    if (!fp) {
        printf("Can't open file.\n");
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    unsigned long fsize = ftell(fp);
    rewind(fp);
    unsigned char *buf = new unsigned char[fsize];
    if (fread(buf, 1, fsize, fp) != fsize) {
        printf("Can't read file.\n");
        delete[] buf;
        return -2;
    }
    fclose(fp);

    // Parse EXIF
    easyexif::EXIFInfo result;
    int code = result.parseFrom(buf, fsize);
    delete[] buf;
    if (code) {
        printf("Error parsing EXIF: code %d\n", code);
        return -3;
    }

    // Dump EXIF information
    _latitude  = result.GeoLocation.Latitude; //纬度
    _longitude = result.GeoLocation.Longitude;  //经度
    _altitude  = result.GeoLocation.Altitude;
    _path = str;

    _para.push_back(to_string(result.GeoLocation.Latitude));
    _para.push_back(to_string(result.GeoLocation.Longitude));
    _para.push_back(to_string(result.GeoLocation.Altitude));


}






