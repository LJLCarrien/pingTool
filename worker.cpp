#include "worker.h"
#include <QDebug>

#include <QProcess>
#include <QRegularExpression>
#include <QThread>


Worker::Worker()
{
    connect(this, &Worker::signal_doGetByUrl, this, &Worker::doGetByUrl);
    connect(this, &Worker::signal_handleIp, this, &Worker::handleIp);
    connect(this, &Worker::signal_resetAll, this, &Worker::resetAll);
    connect(this, &Worker::signal_initAll, this, &Worker::initAll);
}


Worker::~Worker()
{
    resetAll();
}


void Worker::initAll()
{
    resetAll();
    initNetworker();
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

    if(pNetWorker != nullptr)
    {
        pNetWorker = nullptr;
    }
}

void Worker::initNetworker()
{
    if(pNetWorker == nullptr)
    {
        pNetWorker = new NetWorker();
        connect(pNetWorker, &NetWorker::finished, this, &Worker::onReplyFinished);
    }
}

void Worker::doGetByUrl(const QString& rqHost, const QString& ckHost)
{
    debugThreadId("doGetByUrl");

    requestHost = rqHost;
    checkHost = ckHost;

    QString url = QString("https://").append(requestHost).append("/").append(checkHost);
    qDebug() << "[Worker] url : " << url;
    QNetworkReply* reply = pNetWorker->get(url);
    replyEnumMap.insert(reply, NetWorker::RemoteRequest::fetchByUrl);

    qDebug() << "[Worker] replyEnumMap Size: " << replyEnumMap.size();

}


void Worker::doGetCheckIp(const QString& ckHost, IpData data)
{
    if(pNetWorker == nullptr)
    {
        pNetWorker = new NetWorker();
        connect(pNetWorker, &NetWorker::finished, this, &Worker::onReplyFinished);
    }
    debugThreadId("doGetByIp");

    QString url = QString("https://").append(ckHost);
    QString ip = data.ip;

    qDebug() << "[Worker] doGetCheckIp url:" << url;
    QNetworkReply* reply = pNetWorker->getWithHostPort(url, ip);
    replyEnumMap.insert(reply, NetWorker::RemoteRequest::fetchByIp);
    replyIpMap.insert(reply, data);

    qDebug() << "[Worker] replyEnumMap Size: " << replyEnumMap.size();
}

void Worker:: onReplyFinished(QNetworkReply* reply)
{
    // ??????????????????
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
            handleIp(replyStr);
            break;
        }
        case NetWorker::RemoteRequest::fetchByIp:
        {
            qDebug() << "[Worker] onReplyFinished-- fetchByIp:" << QThread::currentThreadId() << QThread::currentThread();

            IpData data = replyIpMap.value(reply);

            QVariant qv = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

            QString code = qv.toString();
            data.checkCode = code;
            setIpData(data);

            bool bValid = qv.isValid();
            qDebug() << QString("[Worker] %1 : %2 : %3").arg(data.ip).arg(bValid).arg(code);
            if(!(data.ip).isEmpty())
            {
                if(qv.isValid())
                {
                    if(code == 200)
                    {
                        okIpList.append(data);
                    }
                    else
                    {
                        qDebug() << (data.ip).append("  Status Code:").append(code);
                    }
                }
                else
                {
                    qDebug() << (data.ip).append(" ??????");
                }
            }
            fetchByIpCount--;
            //??????fetchIp????????????????????????????????????????????????
            if(isExistFetchByIp() && fetchByIpCount == 0)
            {
                showIpData();
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
    debugThreadId("handleIp------");

    QStringList ipList = getIpList(str);

    checkIpIsOk(ipList);
    //    pingIPsByList(ipList);
}


QStringList Worker::getIpList(QString str)
{
    debugThreadId("getIpList");

    // ????????????ip?????????????????? ????????? QStringList
    QRegExp regx(">(\\d+\\.\\d+\\.\\d+\\.\\d+)<");

    QStringList list;
    int pos = 0;

    while((pos = regx.indexIn(str, pos)) != -1)
    {
        list << regx.cap(1);
        pos += regx.matchedLength();
    }


    qDebug() << list.size();
    //    qDebug() << list;
    return list;
}


void Worker::checkIpIsOk(const QStringList list)
{
    int size = list.size();
    qDebug() << "[Worker] checkIpIsOk , size: " << size;
    //    qDebug() << QString::number(size);
    //    qDebug() << ipList;
    fetchByIpCount = size;

    for(int i = 0; i < size; i++)
    {
        QString ip = list.at(i);
        IpData data(ip);
        data.statue = IpStatue::checking;
        ipList.append(data);
        doGetCheckIp(checkHost, data);
    }
    showIpData();
}

void Worker::showIpData()
{
    debugThreadId("showIpData");
    int size = ipList.size();

    for(int i = 0; i < size; i++)
    {
        IpData pItem = ipList.at(i);
        qDebug() << pItem.show();
        emit signal_getIpStr(pItem.show(), i == 0);
    }
}

void Worker::setIpData(IpData data)
{
    int size = ipList.size();

    for(int i = 0; i < size; i++)
    {
        IpData item = ipList.at(i);
        if(item.ip == data.ip)
        {
            ipList.replace(i, data);
            return;
        }
    }
}


void Worker::pingIPsByList(QList<IpData> list)
{
    int size = list.size();
    qDebug() << "[Worker] pingIPsByList , size: " << size;

    qDebug() << QString::number(size);
    //    qDebug() << ipList;

    if(size == 0)
    {
        emit signal_finishHandleIp();
        qDebug() << "??????";
        return;
    }

    IpData ipData;
    for(int i = 0; i < size; i++)
    {
        ipData = list.at(i);
        QString ip = ipData.ip;

        QString stdOut = pingIpForWin(ip);
        QString msValue = getMs(stdOut);

        ipData.ms = msValue;
        ipList.replace(i, ipData);
        //        qDebug() << result;
        emit signal_getIpStr(ipData.show(), i == 0);
        //        emit signal_getIpStr(result, i == 0);

        if(i == size - 1)
        {
            emit signal_finishHandleIp();
            qDebug() << "??????";
        }
    }
}

QString Worker::pingIpForWin(QString ipStr)
{
    //???????????????????????????,????????????win???????????????ping??????
    QProcess pingProcess;
    //    ping??????????????????????????????????????????????????????
    //    -n????????? -i???????????????
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
    //???cmd??????????????????ms
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


void Worker::debugThreadId(QString funcName)
{
    qDebug() << QString("[Worker] %1: ").arg(funcName) << QThread::currentThreadId() << QThread::currentThread();
}

