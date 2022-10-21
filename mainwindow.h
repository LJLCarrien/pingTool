#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    QString win_pingIp(QString ipStr);
    QStringList getIpList(QString txt);
    QString getMs(QString txt);
    QString addMsFormat(QString txt);
    void updateOutputIP(QString txt, bool bclear);

    static bool IsHostOnline(QString strHostName, int nTimeoutmSeconds = 2000);
private slots:
    void on_btnCheck_clicked();

private:
    Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
