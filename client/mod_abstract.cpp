#include "mod_abstract.h"
#include "Client.h"

delta3::ModAbstract::ModAbstract(ProtocolMode mode, qint16 adminId, Client *client)
	: QObject(client), _adminId(adminId), mode_(mode)
{
	connect(this, SIGNAL(messageReadyRead(ProtocolMode,qint16,QByteArray)),
			client, SLOT(sendData3(ProtocolMode,qint16,QByteArray)));

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

