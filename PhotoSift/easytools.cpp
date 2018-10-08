#include <easytools.h>
#include <QDebug>

using namespace std;

std::string GetFilePosfix(const char* path)
{
    const char* pos = strrchr(path,'.');
    if(pos)
    {
        std::string str(pos+1);
        //转换为小写
        std::transform(str.begin(),str.end(),str.begin(),::tolower);
        return str;
    }
    return std::string();
}

bool IsSupportPos(const std::string& posfix,const std::string& support)
{
    std::string str(";");
    str.append(posfix).append(";");
    if(support.find(str) != std::string::npos)
    {
        return true;
    }
    return false;
}

//具体实现
void getFilesContent( string path, vector<string>& files,vector<string>& infos ) {

    long hFile   =   0;  //文件句柄
    //查看一下_finddata结构组成以及_findfirst和_findnext的用法
    struct _finddata_t fileinfo;

    //指定的照片格式
    const string POSFIX = ";jpg;png;jpeg;bmp;gif;";

    string p;
    if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) != -1) {
        do {
            //如果查询到的是子目录,迭代之
            if((fileinfo.attrib & _A_SUBDIR)) {
                if(strcmp(fileinfo.name,".") != 0 && strcmp(fileinfo.name,"..") != 0)
                    getFilesContent( p.assign(path).append("\\").append(fileinfo.name), files,infos );
            }
            else {  //如果不是子目录而是文件，那么就加入列表
                string temStr = GetFilePosfix(fileinfo.name);
                if(IsSupportPos(temStr,POSFIX) == true ){
                    //static int count = 0;
                    //count ++;
                    files.push_back(p.assign(path).append("/").append(fileinfo.name) );
                    infos.push_back(fileinfo.name);
                    //qDebug("这是我读到的第  %d  个文件，文件名是 %s",count,fileinfo.name);
                }
            }
        }
        while(_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}

void replaceSymbol(string &str){
    string::size_type pos(0);
    while((pos = str.find_first_of('\\',pos))!= string::npos){
        str.insert(pos,"\\");
        pos = pos + 2;
    }
}



//替换指定的子串
//src:原字符串 target:待被替换的子串 subs:替换的子串
string replaceALL(const char* src, const string& target,const string& subs)
{
    string tmp(src);
    string::size_type pos =tmp.find(target),targetSize =target.size(),resSize =subs.size();
    while(pos!=string::npos)//found
    {
        tmp.replace(pos,targetSize,subs);
        pos =tmp.find(target, pos + resSize);
    }
    return tmp;
}

QString s2q(const string &s)
{
    return QString(QString::fromLocal8Bit(s.c_str()));
}

string q2s(const QString &s)
{
    return string((const char *)s.toLocal8Bit());
}

