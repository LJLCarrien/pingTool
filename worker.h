#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QMap>

#include "networker.h"
#include "ipdata.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker();
    ~Worker();

private slots:
    void onReplyFinished(QNetworkReply* reply);

signals:
    void signal_resetAll();
    void signal_initAll();

    void signal_doGetByUrl(const QString& rqHost, const QString& ckHost);
    void signal_handleIp(const QString&  str);

    void signal_getIpStr(QString txt, bool bclear);
    void signal_finishHandleIp();

private :
    QString requestHost;
    QString checkHost;
    NetWorker* pNetWorker;
    QMap<QNetworkReply*, NetWorker::RemoteRequest>replyEnumMap;
    int fetchByIpCount;
    QMap<QNetworkReply*, IpData>replyIpMap;
    QList<IpData> okIpList;
    QList<IpData> ipList;

    void initNetworker();
    void resetAll();
    void initAll();

    void doGetByUrl(const QString& rqHost, const QString& ckHost);
    void doGetCheckIp(const QString& checkHost,  IpData data);

    bool isExistFetchByIp();

    void handleIp(const QString&  str);
    QStringList getIpList(QString txt);
    void checkIpIsOk(const QStringList ipList);

    void pingIPsByList(QList<IpData> ipList);
    QString pingIpForWin(QString ipStr);
    QString getMs(QString txt);

    void debugThreadId(QString funcName);
    void showIpData();
    void setIpData(IpData data);
};

#endif // WORKER_H
