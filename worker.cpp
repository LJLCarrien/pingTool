#include "worker.h"
#include <QDebug>

#include <QProcess>
#include <QRegularExpression>
#include <QThread>


Worker::Worker() : QObject()
{
    netWorker = NetWorker::instance();
    connect(netWorker, &NetWorker::finished, this, &Worker::onReplyFinished);
}

void Worker::doGetByUrl(const QString& rqHost, const QString& ckHost)
{
    debugThreadId("doGetByUrl");

    requestHost = rqHost;
    checkHost = ckHost;

    QString url = QString("https://").append(requestHost).append("/").append(checkHost);
    qDebug() << "[Worker] url : " << url;
    QNetworkReply* reply = netWorker->get(url);
    replyEnumMap.insert(reply, NetWorker::RemoteRequest::fetchByUrl);

    qDebug() << "[Worker] replyEnumMap Size: " << replyEnumMap.size();

}


void Worker::doGetCheckIp(const QString& ckHost, const QString& ip)
{
    debugThreadId("doGetByIp");

    QString url = QString("https://").append(ckHost);

    qDebug() << "[Worker] doGetCheckIp url:" << url;
    QNetworkReply* reply = netWorker->getWithHostPort(url, ip);
    replyEnumMap.insert(reply, NetWorker::RemoteRequest::fetchByIp);
    replyIpMap.insert(reply, ip);

    qDebug() << "[Worker] replyEnumMap Size: " << replyEnumMap.size();
}

void Worker:: onReplyFinished(QNetworkReply* reply)
{
    // 获取响应信息
    debugThreadId("onReplyFinished");

    QString replyStr  = reply->readAll();

    if(replyEnumMap.isEmpty())
    {
        return;
    }
    NetWorker::RemoteRequest enumValueReq = replyEnumMap.value(reply);
    //    qDebug() << enumValueReq;
    switch(enumValueReq)
    {

        case NetWorker::RemoteRequest::fetchByUrl:
        {

            qDebug() << "[Worker] onReplyFinished-- fetchByUrl:" << QThread::currentThreadId() << QThread::currentThread();

            //            qDebug() << replyStr;

            emit signal_finishReply(replyStr);
            break;
        }
        case NetWorker::RemoteRequest::fetchByIp:
        {
            qDebug() << "[Worker] onReplyFinished-- fetchByIp:" << QThread::currentThreadId() << QThread::currentThread();

            QString ip = replyIpMap.value(reply);

            QVariant qv = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

            QString code = qv.toString();
            bool bValid = qv.isValid();
            qDebug() << QString("[Worker] %1 : %2 : %3").arg(ip).arg(bValid).arg(code);
            if(!ip.isEmpty())
            {
                if(qv.isValid())
                {
                    okIpList.append(ip);
                    //                    if(code == 200)
                    //                    {
                    //                        okIpList.append(ip);
                    //                    }
                    //                    else
                    //                    {
                    //                        qDebug() << ip.append("  Status Code:").append(code);
                    //                    }
                }
                else
                {
                    qDebug() << ip.append(" 无效");
                }
            }
            fetchByIpCount--;
            //所有fetchIp都已返回，直到返回的是最后才触发
            if(isExistFetchByIp() && fetchByIpCount == 0)
            {
                pingIPsByList(okIpList);
            }
            break;
        }
        default:
            break;
    }

    replyEnumMap.remove(reply);
    if(!replyEnumMap.isEmpty())
    {
        qDebug() << "[Worker] replyEnumMap Size: " << replyEnumMap.size();
    }
    else
    {
        qDebug() << "[Worker] replyEnumMap Size: 0" ;
    }
    reply->deleteLater();
}

bool Worker::isExistFetchByIp()
{
    QList<NetWorker::RemoteRequest> list = replyEnumMap.values();

    int size = list.size();
    for(int i = 0; i < size; i++)
    {
        NetWorker::RemoteRequest item = list.at(i);
        qDebug() << item;
        if(item == NetWorker::RemoteRequest::fetchByIp)
        {
            return true;
        }
    }
    return false;
}


void Worker::handleIp(const QString& str)
{
    debugThreadId("handleIp");

    QStringList ipList = getIpList(str);

    checkIpIsOk(ipList);
    //    pingIPsByList(ipList);
}


QStringList Worker::getIpList(QString str)
{
    debugThreadId("getIpList");

    // 换行分割ip的字符串文本 转换成 QStringList
    QRegExp regx(">(\\d+\\.\\d+\\.\\d+\\.\\d+)<");

    QStringList list;
    int pos = 0;

    while((pos = regx.indexIn(str, pos)) != -1)
    {
        list << regx.cap(1);
        pos += regx.matchedLength();
    }


    qDebug() << list.size();
    qDebug() << list;
    return list;
}

void Worker::resetAll()
{
    if(!replyEnumMap.isEmpty())
    {
        replyEnumMap.clear();
    }
    if(!replyIpMap.isEmpty())
    {
        replyIpMap.clear();
    }
    if(!okIpList.isEmpty())
    {
        okIpList.clear();
    }
    fetchByIpCount = 0;
    requestHost = "";
    checkHost = "";

}

void Worker::checkIpIsOk(const QStringList ipList)
{
    int size = ipList.size();
    qDebug() << "[Worker] checkIpIsOk , size: " << size;
    //    qDebug() << QString::number(size);
    //    qDebug() << ipList;
    fetchByIpCount = size;

    for(int i = 0; i < size; i++)
    {
        QString ip = ipList.at(i);
        doGetCheckIp(checkHost, ip);
    }
}


void Worker::pingIPsByList(QStringList ipList)
{
    int size = ipList.size();
    qDebug() << "[Worker] pingIPsByList , size: " << size;

    qDebug() << QString::number(size);
    qDebug() << ipList;

    if(size == 0)
    {
        emit signal_finishHandleIp();
        qDebug() << "完成";
        return;
    }

    for(int i = 0; i < size; i++)
    {
        QString ip = ipList.at(i);

        QString stdOut = pingIpForWin(ip);
        QString msValue = getMs(stdOut);

        msValue = formatMs(msValue);
        QString result = QString("%1 %2").arg(ip).arg(msValue);

        //        qDebug() << result;
        emit signal_getIpStr(result, i == 0);

        if(i == size - 1)
        {
            emit signal_finishHandleIp();
            qDebug() << "完成";
        }
    }
}

QString Worker::pingIpForWin(QString ipStr)
{
    //这种方法不能跨平台,仅适用于win系统，调用ping命令
    QProcess pingProcess;
    //    ping命令也可以提供用户选择，目前暂时写死
    //    -n是次数 -i是生存时间
    QString strArg = "ping " + ipStr + " -n 1";
    //    qDebug() << strArg;
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

void Worker::debugThreadId(QString funcName)
{
    qDebug() << QString("[Worker] %1: ").arg(funcName) << QThread::currentThreadId() << QThread::currentThread();
}

