#include "MyThread.h"
#include <QDebug>
#include <QThread>
#include <QFile>

MyThread::MyThread(QObject *parent) : QObject(parent)
{
    GPSData d;
    isStop = true;
    isInit = false;
}

void MyThread::setFlag(bool flagIsStop, bool flagIsInit){
    isStop = flagIsStop;
    isInit = flagIsInit;
}

void MyThread::dealSignal(GPSData & d){
    isInit = false;
    gpsData = d; //初始化 gps 数据
    qDebug(">>> 初始化参数已经到达子线程...");
    isInit = true;
}

int MyThread::runTask(){

    while(isStop == false){   //线程未停止时执行
        while(isInit == true){  //初始化后执行
            qDebug()<<">>> 子线程已经收到来自UI的初始化参数，子线程号 ： "<<QThread::currentThread();
            ////////////////////////////////////////////////////////////////
            qDebug()<<">>> 拷贝 / 剪切 / 删除 开始执行... ";

            //【1】提取原始数据
            string path1 = q2s(gpsData.getData(0));   // E:/XXX
            string path2 = q2s(gpsData.getData(1));  // E:/YYY
            path2 = path2 + "/";  // E:/YYY/
            double dPara1 = stringToNum<double>(q2s(gpsData.getData(2)));
            double dPara2 = stringToNum<double>(q2s(gpsData.getData(3)));
            double dPara3 = stringToNum<double>(q2s(gpsData.getData(4)));
            double dPara4 = stringToNum<double>(q2s(gpsData.getData(5)));

            //【2】生成所路的路径名，文件名
            vector<string> files, infos;  //保存文件总数和文件名称（不包括子文件夹名称）
            vector<int> failedFiles , occupiedFiles;
            files.clear(); infos.clear();
            failedFiles.clear(); occupiedFiles.clear();
            getFilesContent(path1, files ,infos);
            //此后不要再修改 files[i] 的格式了,照片 files[i] 的格式此刻为 E:/file/file.txt

            //【3】准备生成相关报告
            ofstream ofs("sel_photo.txt",ios::out);  //.txt
            ofstream oFile;  //.csv
            oFile.open("gps_csv.csv", ios::out | ios::trunc);

            //【4】准备开始执行
            int iSuitCondition = 0;  //符合条件的照片总数
            int iFailed = 0;      //操作失败的照片数
            int iOccupied;  //文件是否被占用
            QString flag = gpsData.getFlag();   //记录当前是执行 复制/剪切/删除 操作

            for( int i = 0; i < files.size(); i++ ) {

                //【5】获取 第i张 照片的EXIF 信息
                GPS gps;
                gps.readGPSFromPhoto(files[i]);

                if(gps.cmpGPS(dPara1,dPara2,dPara3,dPara4)){
                    iSuitCondition++;
                    string dPath2 = path2 + infos[i];  //操作后文件的重命名

                    //【6】执行 复制 / 剪切 / 删除 操作
                    QFile file(s2q(files[i]));
                    if(file.exists()){
                        try{
                            if(flag.compare(QString("COPY")) == 0){
                                if(!QFile::copy(s2q(files[i]),s2q(dPath2))){
                                    iFailed++;
                                    failedFiles.push_back(i);
                                    return -1;
                                }
                            }
                            else if(flag.compare(QString("PASTE")) == 0){
                                if(!QFile::copy(s2q(files[i]),s2q(dPath2))){
                                    iFailed++;
                                    failedFiles.push_back(i);
                                    return -2;
                                }
                                if(!QFile::remove(s2q(files[i]))){
                                    iFailed++;
                                    failedFiles.push_back(i);
                                    return -2;
                                }
                            }
                            else{
                                if(!QFile::remove(s2q(files[i]))){
                                    iFailed++;
                                    failedFiles.push_back(i);
                                    return -3;
                                }
                            }
                        }catch(int){
                            qDebug()<<QString(">>> 部分任务失败，请检查！！！");
                        }catch(...){
                            qDebug()<<QString(">>> TMD，老子也不知道什么错误！！！");
                        }

                        ofs<<gps;
                        oFile << gps.getPara(0)<< ","
                              << gps.getPara(1)<<","
                              << gps.getPara(2)<< endl;

                        //【7】给UI线程发送改变进度条的信号
                        emit signalChangeUI((int)((float(i)/files.size())*100));  //更新UI线程进度条
                    }
                    else{
                        iOccupied++;
                        occupiedFiles.push_back(i);
                    }
                }
            }

            //qDebug("【操作报告】");
            //qDebug(" · 任务总数  %d  张", iSuitCondition);
            //qDebug(" · 成功  %d  张 ， 文件被 占用 / 删除  %d  张 ， 失败  %d  张",iSuitCondition - iFailed - iOccupied , iOccupied ,iFailed);

            ofs.close();
            oFile.close();

            qDebug()<<">>> 拷贝 / 剪切 / 删除 结束 ... ";
            ////////////////////////////////////////////////////////////////
            emit signalDone(files, infos,failedFiles, occupiedFiles);
            isInit = false;
            break;
        }
        if(true == isStop){
            qDebug()<<"子线程号运行结束 ...";
            break;
        }
    }

    return  0;
}
