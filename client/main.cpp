#include <QtGui/QApplication>
#include "UI/mainwindow.h"
#include "Client.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP-866"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	delta3::Client client;

	delta3::MainWindow w(&client);
    return a.exec();
}
