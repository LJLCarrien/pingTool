#include "controller.h"

Controller::Controller() : QObject()
{
}

Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
}

void Controller::beginWork(QString url)
{
    printfThread("handleUrl");

    worker = new Worker(url);
    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);

    connect(worker, &Worker::signal_getIpStr, this, &Controller::signal_updateOutPut);
    connect(worker, &Worker::signal_finishHandleIp, this, &Controller::signal_finishWork);

    workerThread.start();

}

void Controller::printfThread(QString str)
{
    qDebug() << str << ": current thread ID: " << QThread::currentThreadId();
}
