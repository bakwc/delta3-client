#include "mod_abstract.h"
#include "client.h"

delta3::ModAbstract::ModAbstract(ProtocolMode mode, qint16 adminId, Client *client)
    : QObject(client), adminId_(adminId), mode_(mode), client_(client)
{
	connect(this, SIGNAL(messageReadyRead(ProtocolMode,qint16,QByteArray)),
            client, SLOT(sendLevelTwo(ProtocolMode,qint16,QByteArray)));

	qDebug("Activation %i mode", mode_);
}

delta3::ModAbstract::~ModAbstract()
{
	disconnect();

	qDebug("Deactivation %i mode", mode_);
}

/*
delta3::mod_abstract::getAdminId()                          const
{
    return _adminId;
}
*/

