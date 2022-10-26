#include "controller.h"

Controller::Controller() : QObject()
{

    worker = new Worker();
    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    connect(worker, &Worker::signal_getIpStr, this, &Controller::signal_updateOutPut);
    connect(worker, &Worker::signal_finishHandleIp, this, &Controller::signal_finishWork);
    connect(worker, &Worker::signal_finishReply, this, &Controller::handleIp);

}

Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
}

void Controller::doByUrl(const QString& url)
{
    printfThread("doByUrl---------");

    worker->doGetByUrl(url);
    workerThread.start();
}

void Controller::doByUrlIP(const QString& url, const QString& ip)
{
    printfThread("doByUrlIP---------");

    worker->doGetByIp(url, ip);
    workerThread.start();
}

void Controller::handleIp(const QString& reply)
{
    printfThread("handleIp---------");
    worker->handleIp(reply);
}


void Controller::printfThread(QString str)
{
    qDebug() << str << ": current thread ID: " << QThread::currentThreadId() << QThread::currentThread();
}
