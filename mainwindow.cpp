#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    workCtrl = new Controller;
    connect(workCtrl, SIGNAL(signal_updateOutPut(QString, bool)), this, SLOT(updateOutputIP(QString, bool)));
    //    connect(workCtrl, SIGNAL(signal_updateBrowser(QString)), this, SLOT(updateTextBrowser(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnCheck_clicked()
{
    QString url = QString("https://site.ip138.com/").append(ui->inputIP->toPlainText());
    qDebug() << "url: " << url;
    emit workCtrl->signal_handleUrl(url);
}

void MainWindow:: printThreadId(QString txt)
{
    qDebug() << txt << QThread::currentThreadId() << QThread::currentThread();
}

void MainWindow::updateOutputIP(QString txt, bool bclear)
{
    if(bclear)
    {
        ui->outputIP->clear();
    }
    ui->outputIP->appendHtml(txt);
}


void MainWindow::updateTextBrowser(QString txt)
{
    ui->textBrowser->setText(txt);

}






