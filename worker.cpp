#include "worker.h"
#include <QDebug>

#include <QProcess>
#include <QRegularExpression>

Worker::Worker(QString url) : QObject()
{
    doWork(url);
}

void Worker::doWork(QString requestUrl)
{
    qDebug() << "doWork ...";
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReplyFinished(QNetworkReply*)));

    QNetworkRequest request;
    request.setUrl(QUrl(requestUrl));
    request.setRawHeader("Content-Type", "text/html");
    request.setRawHeader("Content-Type", "charset=UTF-8");

    manager->get(request);


}

void Worker::onReplyFinished(QNetworkReply* reply)
{

    qDebug() << "onReplyFinished--";
    // 获取响应信息
    replyStr  = reply->readAll();
    //    emit signal_getReplyStr(replyStr);
    reply->deleteLater();

    handleIp();
}


QStringList Worker::getIpList(QString str)
{
    // 换行分割ip的字符串文本 转换成 QStringList
    QRegExp regx(">(\\d+\\.\\d+\\.\\d+\\.\\d+)<");

    QStringList list;
    int pos = 0;

    while((pos = regx.indexIn(str, pos)) != -1)
    {
        list << regx.cap(1);
        pos += regx.matchedLength();
    }

    return list;
}


void Worker::handleIp()
{
    QStringList ipList = getIpList(replyStr);
    handleIpByList(ipList);
}

void Worker::handleIpByList(QStringList ipList)
{

    int size = ipList.size();

    //    int size = 10;
    qDebug() << QString::number(size);
    qDebug() << ipList;

    for(int i = 0; i < size; i++)
    {
        QString ip = ipList.at(i);
        QString stdOut = win_pingIp(ip);
        QString msValue = getMs(stdOut);

        msValue = formatMs(msValue);
        QString result = QString("%1 %2").arg(ip).arg(msValue);

        qDebug() << result;
        emit signal_getIpStr(result, i == 0);

        if(i == size - 1)
        {
            emit signal_finishHandleIp();
            emit signal_getReplyStr(replyStr);
        }
    }
}

QString Worker::win_pingIp(QString ipStr)
{
    //这种方法不能跨平台,仅适用于win系统，调用ping命令
    QProcess pingProcess;
    //    ping命令也可以提供用户选择，目前暂时写死
    //    -n是次数 -i是生存时间
    QString strArg = "ping " + ipStr + " -n 1";
    qDebug() << strArg;
    pingProcess.start(strArg, QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = QString::fromLocal8Bit(pingProcess.readAllStandardOutput());

    //    qDebug() << p_stdout;
    return p_stdout;
}


QString Worker::getMs(QString txt)
{
    //从cmd命令输出提取ms
    QRegularExpression regx("(?<=\\S=)(\\d+)(?=ms)");
    QRegularExpression::PatternOptions patternOptions = QRegularExpression::DontCaptureOption;
    regx.setPatternOptions(patternOptions);
    QRegularExpressionMatch match = regx.match(txt);

    if(match.hasMatch())
    {
        return match.captured(0);
    }
    return QString::number(0);
}

QString Worker::formatMs(QString ms)
{
    //增加ms格式
    if(QString::number(0) == ms)
    {
        return  QString("<b><font color=\"#FF0000\">%1</font></b>").arg(tr("超时"));
    }
    else
    {
        return QString("<b><font color=\"#0B9B7A\">%1 ms</font></b>").arg(ms);
    }
}

