#include "controller.h"

Controller::Controller() : QObject()
{
    connect(this, SIGNAL(signal_handleUrl(QString)), this, SLOT(handleUrl(QString)));
    connect(this, SIGNAL(signal_HandleIp()), this, SLOT(handleIp()));
}

Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
}

void Controller::handleUrl(QString url)
{
    qDebug() << "beginWork....";

    worker = new Worker(url);
    worker->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(worker, SIGNAL(signal_getIpStr(QString, bool)), this, SLOT(getIpStr(QString, bool)));
    connect(worker, SIGNAL(signal_getReplyStr(QString)), this, SLOT(updateBrowserStr(QString)));
    connect(worker, SIGNAL(signal_finishHandleIp()), this, SLOT(finishWork()));

    workerThread.start();
}

void Controller::handleIp()
{
    worker->handleIp();
}

void Controller:: getIpStr(QString str, bool bclear)
{
    emit(signal_updateOutPut(str, bclear));
}

void Controller::updateBrowserStr(QString str)
{
    emit signal_updateBrowser(str);
}

void Controller::finishWork()
{
    qDebug() << "finishWork...";
    emit signal_finishWork();
}

void Controller::printfThread()
{
    qDebug() << "current thread ID: " << QThread::currentThreadId();
}
