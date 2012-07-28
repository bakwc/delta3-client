#pragma once

#include <QObject>
#include <QByteArray>
#include "defines.h"

namespace delta3
{

class Client;

class ModAbstract: public QObject
{
    Q_OBJECT

public:
	explicit ModAbstract(ProtocolMode mode, qint16 adminId, Client *client);
	~ModAbstract();
    virtual void incomeMessage(const QByteArray &) = 0;

//    quint16 getAdminId()                                        const;

signals:
	void messageReadyRead(ProtocolMode, qint16, QByteArray);

protected:
	qint16		_adminId;
	ProtocolMode mode_;
};

} // namespace delta3

