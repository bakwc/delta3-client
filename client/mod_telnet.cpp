#include "mod_telnet.h"
#include <QDebug>
mod_telnet::mod_telnet(QObject *parent, quint32 adminId_):
    QObject(parent),
    adminId(adminId_)
{
    protocol = new QProcess(this);
#ifdef Q_WS_X11
    protocol->start("bash");
#endif

#ifdef Q_WS_MAC
    protocol->start("bin/bash");
#endif

#ifdef Q_WS_QWS
    protocol->start("bash");
#endif

#ifdef Q_WS_WIN
    protocol->start("cmd");
#endif
    connect(protocol, SIGNAL(readyReadStandardOutput()), this, SLOT(protocolMessage()));
}

void mod_telnet::incomeMessage(const QByteArray &data)
{
    qDebug() << "incomeMessage() \n" << data;
    if(protocol->Running)
        protocol->write(data + '\n');
}

void mod_telnet::close()
{
    protocol->disconnect(this);
    protocol->close();
}

void mod_telnet::protocolMessage()
{
    QString output = QString::fromLocal8Bit(protocol->readAllStandardOutput());
    QByteArray data = (QString("1:%1:%2:").arg(output.size()).arg(output)).toUtf8();
    emit messageReadyRead(adminId, data);
}
