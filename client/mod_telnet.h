#pragma once

#include <QObject>
#include <QProcess>
#include "utils.h"
#include "mod_abstract.h"
#include "defines.h"

namespace delta3
{
	class ModTelnet : public ModAbstract
    {
        Q_OBJECT

    public:
		explicit ModTelnet(qint16 adminId, Client *client);
		~ModTelnet();

        enum TelnetMode {
            TMOD_REQ    = 1,
            TMOD_RESP   = 2,
            TMOD_CMD    = 3
        };

        enum TMODCMD {
            TMODCMD_BREAK   = 1,
            TMODCMD_UP      = 11,
            TMODCMD_DOWN    = 12
        };

        void incomeMessage(const QByteArray &data);
//        quint16 adminId; // in mod_abstract
    public slots:
        void protocolMessage();

    private:
        QProcess* _protocol;
        QString   _prompt;

        QProcess proc_;
        QString createPrompt();
        QByteArray createData(const QString &data) {
            QByteArray send, arr = data.toUtf8();
            send.append(toBytes<quint32>(arr.size()));
            send.append(arr);
            return send;
        }
    };
}
