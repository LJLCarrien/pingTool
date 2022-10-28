#include "controller.h"

Controller::Controller() : QObject()
{

    worker = new Worker();
    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    connect(worker, &Worker::signal_getIpStr, this, &Controller::signal_updateOutPut);
    connect(worker, &Worker::signal_finishHandleIp, this, &Controller::signal_finishWork);
}

Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
}

void Controller::doByUrl(const QString& requestHost, const QString& checkHost)
{
    printfThread("doByUrl---------");

    emit worker->signal_initAll();
    emit worker->signal_doGetByUrl(requestHost, checkHost);
    workerThread.start();
}

void Controller::printfThread(QString str)
{
    qDebug()  << QString("[Controller] %1: ").arg(str) << QThread::currentThreadId() << QThread::currentThread();
}
