#include "mainwindow.h"
#include "ui_mainwindow.h"


namespace delta3{
    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        cl = new Client(this);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }
}

