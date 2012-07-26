#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace delta3{
	MainWindow::MainWindow(Client *client, QWidget *parent) :
		QMainWindow(parent), cl(client), ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        QPixmap oPixmap(16,16);
        oPixmap.fill(qRgb( 255, 0, 255 ));
        QIcon oIcon( oPixmap );
        myIco = new QSystemTrayIcon(this);
        connect(myIco, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this,   SLOT(showHide(QSystemTrayIcon::ActivationReason)));
        myIco->setIcon(oIcon);
        myIco->show();
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }


	void MainWindow::setClient(Client *client)
	{
		cl = client;
	}


    void MainWindow::showHide(QSystemTrayIcon::ActivationReason r) {
        if (r == QSystemTrayIcon::DoubleClick)
            this->close();
    }
}

