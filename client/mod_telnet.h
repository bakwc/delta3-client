#pragma once

#include <QObject>
#include <QProcess>

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
//        QProcess * protocol;
        void incomeMessage(const QByteArray &data);
//        quint16 adminId; // in mod_abstract
    public slots:
        void protocolMessage();

    private:
        QProcess* _protocol;
        QString prompt;
    };
}
