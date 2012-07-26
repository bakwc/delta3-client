#include <QtCore/QCoreApplication>
#include "client.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP-866"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    delta3::Client client;

    return a.exec();
}
