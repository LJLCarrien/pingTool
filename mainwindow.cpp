#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    workCtrl = new Controller;
    connect(workCtrl, &Controller::signal_updateOutPut, this, &MainWindow::updateOutputIP);
    connect(workCtrl, &Controller::signal_finishWork, this, [ = ]()
    {
        updateOutputIP("-----完成-----", false);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCheck_clicked()
{
    printThreadId("on_btnCheck_clicked");
    //    QString url = QString("https://site.ip138.com/").append(ui->inputIP->toPlainText());
    QString url = QString("https://site.ip138.com/files.tmdb.org");
    //    qDebug() << "url: " << url;
    workCtrl->doByUrl(url);
    //    workCtrl->doByUrlIP(url, "147.92.47.123");
}

void MainWindow:: printThreadId(QString txt)
{
    qDebug() << QString("[MainWIndow] %1: ").arg(txt) << QThread::currentThreadId() << QThread::currentThread();
}

void MainWindow::updateOutputIP(QString txt, bool bclear)
{
    if(bclear)
    {
        ui->outputIP->clear();
    }
    ui->outputIP->appendHtml(txt);
}






