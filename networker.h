#ifndef NETWORKER_H
#define NETWORKER_H

#include <QObject>
#include <QNetworkReply>
class NetWorker : public QObject
{
    Q_OBJECT
public:
    static NetWorker* instance();
    ~NetWorker();

    void get(const QString& url);

signals:
    void finished(QNetworkReply* reply);

public slots:

private:
    class Private
    {
    public:
        Private(NetWorker* q): manager(new QNetworkAccessManager(q)) {}
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
