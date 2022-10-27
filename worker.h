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
    void doGetByUrl(const QString& rqHost, const QString& ckHost);
    void doGetCheckIp(const QString& checkHost, const QString& ip);
    void handleIp(const QString&  str);
    void resetAll();


private slots:
    void onReplyFinished(QNetworkReply* reply);

signals:
    void signal_finishReply(QString reply);
    void signal_getIpStr(QString txt, bool bclear);

    void signal_finishHandleIp();

private :
    QString requestHost;
    QString checkHost;
    NetWorker* netWorker;
    QMap<QNetworkReply*, NetWorker::RemoteRequest>replyEnumMap;
    int fetchByIpCount;
    QMap<QNetworkReply*, QString>replyIpMap;
    QList<QString> okIpList;

    QString pingIpForWin(QString ipStr);
    QStringList getIpList(QString txt);

    void pingIPsByList(QStringList ipList);

    void checkIpIsOk(const QStringList ipList);
    bool isExistFetchByIp();

    QString getMs(QString txt);
    QString formatMs(QString txt);

    void debugThreadId(QString funcName);
};

#endif // WORKER_H
