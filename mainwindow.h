#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QProcess>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QNetworkReply>
#include <QStringList>
#include <QTextCodec>

#include "controller.h"



namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void  printThreadId(QString);

private slots:
    void on_btnCheck_clicked();
    void updateOutputIP(QString, bool);
    void updateTextBrowser(QString);

private:
    Ui::MainWindow* ui;
    Controller* workCtrl;

signals:
    void signal_updateUIBrowser(QString);
    void signal_updateUIOutPut(QString txt, bool bclear);
    void signal_endThread();
};

#endif // MAINWINDOW_H
