#include "networker.h"

NetWorker::NetWorker()
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &NetWorker::finished);

    debugThreadId("NetWorker");
}

NetWorker::~NetWorker()
{
    delete manager;
}

QNetworkReply* NetWorker::get(const QString& url)
{
    debugThreadId("get");
    return manager->get(QNetworkRequest(QUrl(url)));
}

QNetworkReply* NetWorker::getWithHostPort(const QString& url, const QString& host, int port)
{
    debugThreadId("getWithHostPort");

    QUrl qUrl = QUrl(url);

    QNetworkProxy proxy;
    proxy.setPort(port);
    proxy.setHostName(host);
    manager->setProxy(proxy);

    QNetworkRequest req(qUrl);

    return manager->get(req);
}


void NetWorker::debugThreadId(const QString& funcName)
{
    qDebug() << QString("[NetWorker] %1: ").arg(funcName) << QThread::currentThreadId() << QThread::currentThread();
}
