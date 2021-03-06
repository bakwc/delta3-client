#include <QtGui/QApplication>
#include "UI/mainwindow.h"
#include "client.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

#if defined(Q_OS_WIN)
QTextCodec::setCodecForLocale(QTextCodec::codecForName("cp866"));
#endif

    delta3::Client client(QHostAddress::LocalHost);

	delta3::MainWindow w(&client);
    return a.exec();
}
