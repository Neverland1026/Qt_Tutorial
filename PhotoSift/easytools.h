#ifndef EASYTOOLS_H
#define EASYTOOLS_H

#include <fstream>
#include <windows.h>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <io.h>
#include <string.h>

#include <algorithm>
#include <assert.h>

#include <QString>

using namespace std;

//前向声明
void getFilesContent( string path, vector<string>& files,vector<string>& infos );

void replaceSymbol(string &str);

//模板函数：将string类型变量转换为常用的数值类型（此方法具有普遍适用性）
template <class Type>
Type stringToNum(const string& str)
{
    istringstream iss(str);
    Type num;
    iss >> num;
    return num;
}

string replaceALL(const char* src, const string& target,const string& subs);

QString s2q(const string &s);

string q2s(const QString &s);


std::string GetFilePosfix(const char* path) ;


bool IsSupportPos(const std::string& posfix,const std::string& support) ;


#endif // EASYTOOLS_H
