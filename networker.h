#ifndef NETWORKER_H
#define NETWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QThread>

class NetWorker : public QObject
{
    Q_OBJECT

public: NetWorker();
    ~NetWorker();

    QNetworkReply* get(const QString& url);
    QNetworkReply* getWithHostPort(const QString& url, const QString& host, int port = 80);
    void debugThreadId(const QString& funcName);

    enum RemoteRequest
    {
        fetchByUrl,
        fetchByIp
    };

private:
    QNetworkAccessManager* manager;

signals:
    void finished(QNetworkReply* reply);
};

#endif // NETWORKER_H

