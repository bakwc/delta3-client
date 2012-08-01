#include "mod_telnet.h"
#include <QDebug>

namespace delta3
{
ModTelnet::ModTelnet(qint16 adminId, Client *client)
        : ModAbstract(MOD_TELNET, adminId, client)
    {
        _protocol = new QProcess(this);
    #ifdef Q_WS_X11                             // linux desktop
        //constructing command prompt
        QByteArray username,hostname,cwd;
        QProcess username_proc;
        QProcess hostname_proc;
        QProcess cwd_proc;
        username_proc.start("whoami");
        username_proc.waitForFinished();
        username = username_proc.readAll();
        //qDebug() << username;
        hostname_proc.start("hostname");
        hostname_proc.waitForFinished();
        hostname = hostname_proc.readAll();
        //qDebug() << hostname;
        cwd_proc.start("pwd");
        cwd_proc.waitForFinished();
        cwd = cwd_proc.readAll();
        username_proc.close();
        hostname_proc.close();
        cwd_proc.close();
        prompt = QString::fromUtf8(username);
        prompt.chop(1);
        prompt.append("@");
        prompt.append(hostname);
        prompt.chop(1);
        prompt.append(" ");
        prompt.append(cwd);
        prompt.chop(1);
        prompt.append("$ ");
        //qDebug() << "Prompt: " << prompt;
        _protocol->start("/bin/sh");
    #elif defined(Q_WS_MAC)                     // darwin
        _protocol->start("/bin/bash");
    #elif defined(Q_WS_QWS)                     // embedded linux
        _protocol->start("bash");
    #elif defined(Q_OS_MSDOS)
        _protocol->start("cmd");
    #elif defined(Q_WS_WIN)
        _protocol->start("cmd");
    #else
        qDebug() << "This OS is not supported";
    #endif
        connect(_protocol, SIGNAL(readyReadStandardOutput()),
                this, SLOT(protocolMessage()));
        emit messageReadyRead(MOD_TELNET, _adminId, prompt.toUtf8());
}

    ModTelnet::~ModTelnet()
    {
        _protocol->disconnect(this);
        _protocol->close();
    }

    void ModTelnet::incomeMessage(const QByteArray &data)
    {
        QString cmd = QString::fromUtf8(data);
        cmd.append('\n');
        qDebug() << cmd ;
        if(_protocol->Running)
            _protocol->write(cmd.toLocal8Bit());
    }

    void ModTelnet::protocolMessage()
    {
        QString output = QString::fromLocal8Bit(_protocol->readAllStandardOutput() + "\n" + prompt.toLocal8Bit());
        qDebug() << "proto3 output:\n" << output.toUtf8();
        emit messageReadyRead(MOD_TELNET, _adminId, output.toUtf8());
    }
}
