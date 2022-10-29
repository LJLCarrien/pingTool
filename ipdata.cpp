#include "ipdata.h"

IpData::IpData(): ip(""), ms(""), checkCode(""), statue(IpStatue::waiting)
{

}

IpData::IpData(QString str, IpStatue s): ip(str), ms(""), checkCode(""), statue(s)
{

}

QString IpData::addColor(QString str, Color c)
{
    QString result;
    switch(c)
    {
        case Color::Green:
            result = QString("<b><font color=\"#0B9B7A\">%1 ms</font></b>").arg(str);
            break;
        case Color::Red:
            result = QString("<b><font color=\"#FF0000\">%2 ms</font></b>").arg(str);
            break;
        case Color::Orange:
            result = QString("<b><font color=\"#FF9400\">%2 ms</font></b>").arg(str);
            break;
        case Color::Blue:
            result = QString("<b><font color=\"#049CD5\">%2 ms</font></b>").arg(str);
            break;
        default:
            break;
    }
    return result;
}

QString IpData::getCheckCode()
{
    Color c = Color::Green;
    if(checkCode != "200")
    {
        c = Color::Red;
    }

    return addColor(checkCode, c);
}

QString IpData::getMs()
{
    Color c = Color::Green;
    int ims = ms.toInt();
    if(ims > 100)
    {
        c = Color::Blue;
    }
    else if(ims > 200)
    {
        c = Color::Orange;
    }
    return addColor(QString("%1 ms").arg(ms), c);
}

QString IpData::show()
{
    QString checkStr = "-";
    QString pingStr = "-";
    QString msStr = "-";

    if(statue == IpStatue::waiting)
    {
        checkStr = "待处理";
    }
    else if(statue == IpStatue::checking)
    {
        checkStr = "请求中";
    }

    if(!checkCode.isEmpty())
    {
        checkStr = QString("状态码: ").append(getCheckCode());
    }
    if(!ms.isEmpty())
    {

        msStr = getMs();
    }

    if(statue == IpStatue::Success)
    {
        pingStr = "成功";
    }
    else if(statue == IpStatue::Fail)
    {
        pingStr = "<b><font color=\"#FF0000\">超时</font></b>";
    }
    QString str = QString("%1 %2 %3 %4").arg(ip).arg(checkStr).arg(pingStr).arg(msStr);

    return str;
}
