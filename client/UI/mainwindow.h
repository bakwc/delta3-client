#pragma once

#include "client.h"
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

	explicit MainWindow(Client *client = NULL, QWidget *parent = 0);
    ~MainWindow();

	void setClient(Client *client);
    
private slots:
     void showHide(QSystemTrayIcon::ActivationReason);

private:
     Client *_cl;
     QSystemTrayIcon *_myIco;
     Ui::MainWindow *_ui;
};

}
