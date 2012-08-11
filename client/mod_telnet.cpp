#include "mod_telnet.h"
#include <client.h>
#include <QtAlgorithms>
#include <QDebug>

namespace delta3
{
ModTelnet::ModTelnet(qint16 adminId, Client *client)
        : ModAbstract(MOD_TELNET, adminId, client)
    {
        _protocol = new QProcess(this);
    #ifdef Q_WS_X11                             // linux desktop
        //constructing command prompt
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

#ifdef Q_WS_X11
        QByteArray send;
        send.append((quint8)TMOD_RESP);
        send.append( createData(createPrompt()) );
        sendData(send);
#endif
}

    ModTelnet::~ModTelnet()
    {
        _protocol->disconnect(this);
        _protocol->close();
    }

    void ModTelnet::incomeMessage(const QByteArray &data)
    {
        switch ((quint8)data[0]) {
        case TMOD_REQ: {
            int size = fromBytes<quint32>(data.mid(1, 4));
            QString cmd = QString::fromUtf8(data.mid(5, size)) + '\n';

            if (_protocol->Running)
                _protocol->write(cmd.toLocal8Bit());
        }
        }
    }

    void ModTelnet::protocolMessage()
    {
        #if defined(Q_OS_WIN)
        QString output = QString::fromLocal8Bit(
                    _protocol->readAllStandardOutput());
        #else
        QString output = QString::fromLocal8Bit(_protocol->readAll()) + createPrompt();
        #endif

        QByteArray send;
        send.append((quint8)TMOD_RESP);
        send.append( createData(output) );

        sendData(send);
    }


    QString ModTelnet::createPrompt()
    {
        QString str;

        proc_.start("whoami");
        proc_.waitForFinished();
        str.append(proc_.readAll());
        str.chop(1);
        str.append('@');

        proc_.start("hostname");
        proc_.waitForFinished();
        str.append(proc_.readAll());
        str.chop(1);
        str.append(':');

        proc_.start("pwdx " + QString::number((qint64)_protocol->pid()));
        proc_.waitForFinished();

        QString pwdx(proc_.readAll());
        pwdx.remove(QRegExp("^[0-9]{1,}: "));
        str.append(pwdx);
        str.chop(1);

        str.append("$ ");

        return str;
    }
}
