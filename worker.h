#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QNetworkReply>

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QString url);

    QString win_pingIp(QString ipStr);
    QStringList getIpList(QString txt);
    void handleIpByList(QStringList ipList);

    QString getMs(QString txt);
    QString formatMs(QString txt);

public slots:
    void doWork(QString requestUrl);
    void onReplyFinished(QNetworkReply* reply);
    void handleIp();

private :
    QString replyStr;

signals:
    void signal_getReplyStr(QString);
    void signal_HandleIp();
    void signal_getIpStr(QString txt, bool bclear);
    void signal_finishHandleIp();
};

#endif // WORKER_H
