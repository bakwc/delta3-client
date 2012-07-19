#include "mod_telnet.h"
#include <QDebug>

namespace delta3
{
    mod_telnet::mod_telnet(QObject *parent, quint16 adminId_):
        QObject(parent),
        adminId(adminId_)
    {
        protocol = new QProcess(this);
    #ifdef Q_WS_X11
        // linux desktop
        protocol->start("bash");
    #elif Q_WS_MAC
        // darwin
        protocol->start("/bin/bash");
    #elif Q_WS_QWS
        // embedded linux
        protocol->start("bash");
    #elif Q_WS_WIN
        protocol->start("cmd");
    #else
        "This OS is not supported"
    #endif
        connect(protocol, SIGNAL(readyReadStandardOutput()), this, SLOT(protocolMessage()));
    }

    void mod_telnet::incomeMessage(const QByteArray &data)
    {

        QString cmd = QString::fromUtf8(data);
        cmd.append('\n');
        qDebug() << cmd ;
        if(protocol->Running)
            protocol->write(cmd.toLocal8Bit());
    }

    void mod_telnet::close()
    {
        protocol->disconnect(this);
        protocol->close();
    }

    void mod_telnet::protocolMessage()
    {
        QString output = QString::fromLocal8Bit(protocol->readAllStandardOutput());
        qDebug() << output;
        emit messageReadyRead(MOD_TELNET, adminId, output.toUtf8());
    }
}
