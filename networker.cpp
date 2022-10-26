#include "networker.h"
#include <QMutex>
NetWorker* NetWorker::instance()
{
    //C++单例模式的最简单写法
    static NetWorker netWorker;
    return &netWorker;
}

NetWorker::~NetWorker()
{
    delete d;
    d = nullptr;
}

QNetworkReply* NetWorker::get(const QString& url)
{
    return d->manager->get(QNetworkRequest(QUrl(url)));
}

QNetworkReply* NetWorker::getByIp(const QString& url, const QString& ip)
{
    QUrl qUrl = QUrl(url);
    QString host = qUrl.host();

    QNetworkProxy proxy;
    proxy.setPort(ip.toShort());
    proxy.setHostName(host);
    d->manager->setProxy(proxy);

    QNetworkRequest req(qUrl);

    return d->manager->get(req);
}

NetWorker::NetWorker(QObject* parent) : QObject(parent), d(new NetWorker::Private(this))
{
    connect(d->manager, &QNetworkAccessManager::finished, this, &NetWorker::finished);
}

