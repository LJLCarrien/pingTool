#ifndef NETWORKER_H
#define NETWORKER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QThread>

class NetWorker : public QObject
{
    Q_OBJECT
public:
    static NetWorker* instance();
    ~NetWorker();

    QNetworkReply* get(const QString& url);
    QNetworkReply* getByIp(const QString& url, const QString& ip);

    enum RemoteRequest
    {
        fetchByUrl,
        fetchByIp
    };

signals:
    void finished(QNetworkReply* reply);

public slots:

private:
    class Private
    {
    public:
        Private(NetWorker* q)
        {
            manager = new QNetworkAccessManager(q);
            qDebug()  << "Private: current thread ID: " << QThread::currentThreadId() << QThread::currentThread();
        }
        QNetworkAccessManager* manager;
    }
    ;
    friend class Private;
    Private* d;

    explicit NetWorker(QObject* parent = 0);
    //将所修饰的函数声明为 deleted（C++11 的新特性）
    NetWorker(const NetWorker&) Q_DECL_EQ_DELETE;
    NetWorker operator=(NetWorker rhs) Q_DECL_EQ_DELETE;

};

#endif // NETWORKER_H
