#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QNetworkReply>
#include "networker.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QString url);
    void doWork(QString requestUrl);

private slots:
    void onReplyFinished(QNetworkReply* reply);

signals:
    void signal_getIpStr(QString txt, bool bclear);
    void signal_finishHandleIp();

private :
    QString replyStr;
    NetWorker* netWorker;

    QString pingIpForWin(QString ipStr);
    QStringList getIpList(QString txt);
    void handleIp(QString str);
    void handleIpByList(QStringList ipList);


    QString getMs(QString txt);
    QString formatMs(QString txt);
};

#endif // WORKER_H
