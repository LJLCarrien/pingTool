#ifndef IPDATA_H
#define IPDATA_H

#include <QString>

enum IpStatue
{
    waiting,
    checking,
    pinging,
    Success,
    Fail
};

enum Color
{
    Red,
    Green,
    Orange,
    Blue
};

class IpData
{
public:
    IpData();
    IpData(QString str, IpStatue s = IpStatue::waiting);

    QString ip;
    QString ms;//ping获取的ms
    QString checkCode;//请求返回状态码
    IpStatue statue;


    QString show();
    QString addColor(QString str, Color c = Color::Green);
    QString getCheckCode();
    QString getMs();
};

#endif // IPDATA_H
