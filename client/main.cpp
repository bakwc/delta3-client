#include <QtGui/QApplication>
#include "UI/mainwindow.h"
#include "client.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP-866"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    delta3::Client client(QHostAddress("193.169.33.254"));

	delta3::MainWindow w(&client);
    return a.exec();
}
