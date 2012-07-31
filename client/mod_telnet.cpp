#include "mod_telnet.h"
#include <QDebug>

namespace delta3
{
ModTelnet::ModTelnet(qint16 adminId, Client *client)
		: ModAbstract(MOD_TELNET, adminId, client)
    {
        _protocol = new QProcess(this);
    #ifdef Q_WS_X11
        // linux desktop
        _protocol->start("/bin/sh");
        QByteArray username,hostname;
        QString username_cmd = "whoami";
        QString hostname_cmd = "hostname";
        QProcess sh;// = new QProcess(this);
        sh.start("/bin/bash");
        if(sh.waitForStarted(10000)){
            qDebug() << sh.state();
        //else if(sh.state() != QProcess::Running) qDebug() << "OLOLO, PUSH PUSH";
        sh.setProcessChannelMode(QProcess::MergedChannels);
        sh.write(username_cmd.toLocal8Bit());
        qDebug() << username_cmd;
        //prompt.waitForFinished();
        username = sh.readAllStandardOutput();
        if(username=="") qDebug()<<"Read1 Error";
        //QString username = QString::fromLocal8Bit(prompt.readAllStandardOutput());
        sh.write(hostname_cmd.toLocal8Bit());
        qDebug() << hostname_cmd;
        hostname = sh.readAllStandardOutput();
        if(hostname=="") qDebug()<<"Read2 Error";
        //QString hostname = QString::fromLocal8Bit(prompt.readAllStandardOutput());
        //sh.disconnect(this);
        sh.close();
        prompt = QString::fromUtf8(username);
        prompt.append("@");
        prompt.append(hostname);
        prompt.append(" $ ");
        qDebug() << "Prompt: " << prompt;}
        else qDebug() << "OLOLO, PUSH PUSH";
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
        QString output = QString::fromLocal8Bit(_protocol->readAllStandardOutput() + "\n> ");
        qDebug() << "proto3 output:\n" << output.toUtf8();
        emit messageReadyRead(MOD_TELNET, _adminId, output.toUtf8());
    }
}
