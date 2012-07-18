#pragma once
#include "Client.h"
#include <QMainWindow>
#include <QSystemTrayIcon>
namespace Ui {
class MainWindow;
}
namespace delta3{

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    Client * cl;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    QSystemTrayIcon *myIco;
    Ui::MainWindow *ui;
private slots:
     void showHide(QSystemTrayIcon::ActivationReason);
};

}
