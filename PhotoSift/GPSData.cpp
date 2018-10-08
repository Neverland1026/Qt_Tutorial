#include "GPSData.h"
#include <QDebug>

GPSData::GPSData()
{

}

GPSData::GPSData(QString para1,QString para2,QString para3,QString para4,QString para5,QString para6,QString para7){
    srcFilePath = para1;
    destFilePath = para2;
    maxLon = para3;
    minLon = para4;
    maxLat = para5;
    minLat = para6;
    flag = para7;
}

QString GPSData::getData(int index){
    if(index >=0 && index <=5)
        switch(index){
        case 0:
            return srcFilePath;
        case 1:
            return destFilePath;
        case 2:
            return maxLon;
        case 3:
            return minLon;
        case 4:
            return maxLat;
        case 5:
            return minLat;
        }
    return NULL;
}

QString GPSData::getFlag(){
    return flag;
}


