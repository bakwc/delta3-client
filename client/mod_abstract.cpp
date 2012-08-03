#include "mod_abstract.h"
#include "client.h"

delta3::ModAbstract::ModAbstract(ProtocolMode mode, qint16 adminId, Client *client)
    : QObject(client), _adminId(adminId), _mode(mode)
{
	connect(this, SIGNAL(messageReadyRead(ProtocolMode,qint16,QByteArray)),
			client, SLOT(sendData3(ProtocolMode,qint16,QByteArray)));

    qDebug("Activation %i mode", _mode);
}

delta3::ModAbstract::~ModAbstract()
{
	disconnect();

    qDebug("Deactivation %i mode", _mode);
}

/*
delta3::mod_abstract::getAdminId()                          const
{
    return _adminId;
}
*/

