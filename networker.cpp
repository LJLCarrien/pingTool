#include "networker.h"

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

void NetWorker::get(const QString& url)
{
    d->manager->get(QNetworkRequest(QUrl(url)));
}

NetWorker::NetWorker(QObject* parent) : QObject(parent), d(new NetWorker::Private(this))
{
    connect(d->manager, &QNetworkAccessManager::finished, this, &NetWorker::finished);
}

