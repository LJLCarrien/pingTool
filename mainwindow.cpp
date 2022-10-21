#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QProcess>
#include<QDebug>
#include<QNetworkRequest>
#include<QNetworkAccessManager>
#include<QTimer>
#include<QNetworkReply>
#include <QStringList>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/**
 * @brief 仅适用于win系统，调用ping命令
 * @param ipStr
 * @return
 */
QString MainWindow::win_pingIp(QString ipStr)
{
    //这种方法不能跨平台
    QProcess pingProcess;
    //    ping命令也可以提供用户选择，目前暂时写死
    //    -n是次数 -i是生存时间
    QString strArg = "ping " + ipStr + " -n 1";
    pingProcess.start(strArg, QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = QString::fromLocal8Bit(pingProcess.readAllStandardOutput());

    qDebug() << p_stdout;

    //    if(p_stdout.contains("TTL="))
    //    {
    //        printf("Ping true\n");
    //    }
    //    else
    //    {
    //        printf("Ping false\n");
    //    }
    return p_stdout;
}

/**
 * @brief 换行分割ip的字符串文本 转换成 QStringList
 * @param str
 * @return
 */
QStringList MainWindow::getIpList(QString str)
{
    QRegExp regx("\\d+.\\d+.\\d+.\\d+");
    QStringList list;
    int pos = 0;
    while((pos = regx.indexIn(str, pos)) != -1)
    {
        list << regx.cap(0);
        pos += regx.matchedLength();


    }
    qDebug() << "list : " << list;
    return list;
}

/**
 * @brief 从结果提取ms
 * @param txt
 * @return
 */
QString MainWindow::getMs(QString txt)
{
    QRegularExpression regx("(?<=\\S=)(\\d+)(?=ms)");
    QRegularExpression::PatternOptions patternOptions = QRegularExpression::DontCaptureOption;
    regx.setPatternOptions(patternOptions);
    QRegularExpressionMatch match = regx.match(txt);

    if(match.hasMatch())
    {
        return match.captured(0);
    }
    return QString::number(0);
}

/**
 * @brief 增加ms格式
 * @param txt
 * @return
 */
QString MainWindow::addMsFormat(QString ms)
{
    if(QString::number(0) == ms)
    {
        return  QString("<b><font color=\"#FF0000\">%1</font></b>").arg(tr("超时"));
    }
    else
    {
        return QString("<b><font color=\"#0B9B7A\">%1 ms</font></b>").arg(ms);
    }
}



/**
 * @brief UI逻辑-更新Output
 * @param txt
 */
void MainWindow::updateOutputIP(QString txt, bool bclear)
{
    if(bclear)
    {
        ui->outputIP->clear();
    }
    ui->outputIP->appendHtml(txt);
}

bool MainWindow::IsHostOnline(QString strHostName, int nTimeoutmSeconds)
{
    QNetworkRequest request(strHostName);
    request.setRawHeader("Content-Type", "charset='utf-8'");
    request.setRawHeader("Content-Type", "application/json");
    QNetworkAccessManager* naManager = new QNetworkAccessManager;

    QEventLoop eventloop;
    QObject* obj_eventloop = &eventloop;

    QTimer timer;
    //    timer.singleShot(nTimeoutmSeconds, &eventloop, SLOT(quit()));
    timer.singleShot(nTimeoutmSeconds, obj_eventloop, SLOT(quit()));
    timer.start();

    QNetworkReply* reply = naManager->get(request);
    QObject* obj_reply = reply;
    //    QMetaObject::Connection conRet = QObject::connect(reply, SIGNAL(finised()), &eventloop, SLOT(quit()), Qt::AutoConnection);
    QMetaObject::Connection conRet = QObject::connect(obj_reply, SIGNAL(finised()), obj_eventloop, SLOT(quit()), Qt::AutoConnection);
    //    Q_ASSERT(conRet);

    eventloop.exec(QEventLoop::ExcludeUserInputEvents);

    if(!timer.isActive())
    {
        //超时，未知状态
        disconnect(reply, SIGNAL(finished()), &eventloop, SLOT(quit()));
        reply->abort();
        reply->deleteLater();

        return false;
    }

    if(reply->error() != QNetworkReply::NoError)
    {
        reply->abort();
        reply->deleteLater();
        return false;
    }

    bool bRes = reply->readAll().length() > 0;
    reply->abort();
    reply->deleteLater();

    return bRes;

}

/**
 * @brief UI逻辑-响应btnCheck点击
 */
void MainWindow::on_btnCheck_clicked()
{
    QString txt = ui->inputIP->toPlainText();
    QStringList list = getIpList(txt);
    for(int i = 0; i < list.size(); i++)
    {
        QString ip = list.at(i);
        QString stdOut = win_pingIp(ip);
        QString msValue = getMs(stdOut);

        msValue = addMsFormat(msValue);
        QString result = QString("%1 %2").arg(ip).arg(msValue);
        updateOutputIP(result, i == 0);

    }

    //    bool bResult = IsHostOnline(ip, 2000);
    //    QString bResultText = bResult ? "True" : "False";
    //    QString txt = QString("%1 : %2").arg(ip).arg(bResultText);
    //    updateOutputIP(txt);

}

