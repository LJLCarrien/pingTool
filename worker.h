#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QMap>

#include "networker.h"

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
    QMap<QNetworkReply*, QString>replyIpMap;
    QList<QString> okIpList;

    void initNetworker();
    void resetAll();
    void initAll();

    void doGetByUrl(const QString& rqHost, const QString& ckHost);
    void doGetCheckIp(const QString& checkHost, const QString& ip);

    bool isExistFetchByIp();

    void handleIp(const QString&  str);
    QStringList getIpList(QString txt);
    void checkIpIsOk(const QStringList ipList);

    void pingIPsByList(QStringList ipList);
    QString pingIpForWin(QString ipStr);
    QString getMs(QString txt);
    QString formatMs(QString txt);

    void debugThreadId(QString funcName);
};

#endif // WORKER_H
