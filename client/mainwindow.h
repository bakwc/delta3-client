#pragma once
#include "Client.h"
#include <QMainWindow>

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
    Ui::MainWindow *ui;
};

}
