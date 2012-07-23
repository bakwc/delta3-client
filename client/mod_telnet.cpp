#include "mod_telnet.h"
#include <QDebug>

namespace delta3
{
    mod_telnet::mod_telnet(QObject *parent, quint16 adminId_)
        : mod_abstract(parent, adminId_)
    {
        _protocol = new QProcess(this);
    #ifdef Q_WS_X11
        // linux desktop
        _protocol->start("bash");
    #elif defined(Q_WS_MAC)
        // darwin
        _protocol->start("/bin/bash");
    #elif defined(Q_WS_QWS)
        // embedded linux
        _protocol->start("bash");
    #elif defined(Q_OS_MSDOS)
        _protocol->start("cmd");
    #elif defined(Q_WS_WIN)
        _protocol->start("cmd");
    #else
        qDebug() << "This OS is not supported";
    #endif
        connect
        (
            _protocol, SIGNAL(readyReadStandardOutput()),
            this, SLOT(protocolMessage())
        );
    }

    void mod_telnet::incomeMessage(const QByteArray &data)
    {

        QString cmd = QString::fromUtf8(data);
        cmd.append('\n');
        qDebug() << cmd ;
        if(_protocol->Running)
            _protocol->write(cmd.toLocal8Bit());
    }

    void mod_telnet::close()
    {
        _protocol->disconnect(this);
        _protocol->close();
    }

    void mod_telnet::protocolMessage()
    {
        QString output = QString::fromLocal8Bit(_protocol->readAllStandardOutput());
        qDebug() << "proto3 output:\n" << output.toUtf8();
        emit messageReadyRead(MOD_TELNET, _adminId, output.toUtf8());
    }
}
