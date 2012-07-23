#pragma once

#include <QObject>
#include <QProcess>

#include "mod_abstract.h"
#include "defines.h"

namespace delta3
{
    class mod_telnet : public mod_abstract
    {
        Q_OBJECT

    public:
        explicit mod_telnet(QObject *parent = 0, quint16 adminId = 0);
//        QProcess * protocol;
        void incomeMessage(const QByteArray &data);
//        quint16 adminId; // in mod_abstract
        void close();
    public slots:
        void protocolMessage();

    signals:
        void messageReadyRead(ProtocolMode, qint16, QByteArray);

    private:
        QProcess* _protocol;
    };
}
