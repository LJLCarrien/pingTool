#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>
#include <qDebug>

#include <worker.h>


class Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller();
    ~Controller();
    void beginWork(QString url);

public slots:

signals:
    void signal_finishWork();
    void signal_updateOutPut(QString str, bool bclear);

private:
    QThread workerThread;
    Worker* worker;

    void printfThread(QString str);
};

#endif // CONTROLLER_H
