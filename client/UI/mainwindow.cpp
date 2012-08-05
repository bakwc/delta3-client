#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace delta3{

	MainWindow::MainWindow(Client *client, QWidget *parent) :
		QMainWindow(parent), _cl(client), _ui(new Ui::MainWindow)
    {
        _ui->setupUi(this);
        QPixmap oPixmap(16,16);
        oPixmap.fill(qRgb( 255, 0, 255 ));
        QIcon oIcon( oPixmap );
        _myIco = new QSystemTrayIcon(this);
        connect(_myIco, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this,   SLOT(showHide(QSystemTrayIcon::ActivationReason)));
        _myIco->setIcon(oIcon);
        _myIco->show();
    }

    MainWindow::~MainWindow()
    {
        delete _ui;
    }


	void MainWindow::setClient(Client *client)
	{
        _cl=client;
	}


    void MainWindow::showHide(QSystemTrayIcon::ActivationReason r)
    {
        if (r == QSystemTrayIcon::DoubleClick)
            this->close();
    }

}

