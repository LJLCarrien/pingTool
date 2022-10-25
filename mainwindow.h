#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QProcess>
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

private slots:
    void on_btnCheck_clicked();

signals:

private:
    Ui::MainWindow* ui;
    Controller* workCtrl;
    void  printThreadId(QString);
    void updateOutputIP(QString, bool);
};

#endif // MAINWINDOW_H
