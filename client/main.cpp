#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP-866"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    delta3::MainWindow w;
    return a.exec();
}
