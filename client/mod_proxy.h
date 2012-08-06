#pragma once

#include <QObject>
#include "mod_abstract.h"
#include "defines.h"

class QTcpSocket;

namespace delta3
{

class Mod_Proxy : public ModAbstract
{
    Q_OBJECT
public:
    explicit Mod_Proxy(qint16 adminId, Client *client);
    void incomeMessage(const QByteArray &data);
    void close();

signals:
    void messageReadyRead(ProtocolMode, qint16,const QByteArray&);

public slots:
    void protocolMessage();

private slots:
    void slotConnected();
    void onDisconnect();

private:
    QTcpSocket *_socket;
    QByteArray _data;
    QByteArray _buff;

    QString getHost(QByteArray data);
    void replaceKeepAlive(QByteArray& data);
};

}
