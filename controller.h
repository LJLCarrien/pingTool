#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>

#include <worker.h>
#include <qDebug>

class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller();
    ~Controller();

    void printfThread();

public slots:
    void handleUrl(QString url);
    void handleIp();
    void finishWork();
    void getIpStr(QString txt, bool bclear);
    void updateBrowserStr(QString txt);


private:
    QThread workerThread;
    Worker* worker;

signals:
    void signal_handleUrl(QString url);
    void signal_HandleIp();
    void signal_finishWork();
    void signal_updateOutPut(QString str, bool bclear);
    void signal_updateBrowser(QString str);
};

#endif // CONTROLLER_H
