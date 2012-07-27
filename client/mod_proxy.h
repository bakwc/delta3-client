#pragma once

#include <QObject>
#include "mod_abstract.h"
#include "defines.h"

class QTcpSocket;

namespace delta3
{

class Mod_Proxy : public mod_abstract
{
    Q_OBJECT
public:
    explicit Mod_Proxy(QObject *parent = 0, quint16 adminId = 0);
    void incomeMessage(const QByteArray &data);
    void close();
signals:
    void messageReadyRead(ProtocolMode, qint16,const QByteArray&);
public slots:
    void protocolMessage();
private:
    QTcpSocket *_socket;
    QString getHost(QByteArray data);
    QByteArray _data;
private slots:
    void slotConnected();
};

}
