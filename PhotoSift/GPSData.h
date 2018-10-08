#ifndef GPSDATA_H
#define GPSDATA_H

//必须包含QMetaType，否则会出现下面错误：
//error: expected constructor, destructor, or type conversion before ‘;’ token
#include <QMetaType>
#include <QString>

class GPSData
{
public:
    GPSData();
    GPSData(QString,QString,QString,QString,QString,QString,QString);

    QString getData(int);
    QString getFlag();

private:
    QString srcFilePath;
    QString destFilePath;
    QString maxLon;
    QString minLon;
    QString maxLat;
    QString minLat;
    QString flag;
};

Q_DECLARE_METATYPE(GPSData) //这是个宏，不能加 ;

#endif // GPSDATA_H
