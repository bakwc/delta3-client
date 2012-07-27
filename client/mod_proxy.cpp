#include <QRegExp>
#include <QDebug>
#include <QTcpSocket>
#include "mod_proxy.h"

namespace delta3
{
Mod_Proxy::Mod_Proxy(QObject *parent, quint16 _adminId) :
    mod_abstract(parent, _adminId)
{
    _socket = new QTcpSocket;

    connect(_socket, SIGNAL(readyRead()), SLOT(protocolMessage()));
    connect(_socket, SIGNAL(connected()), SLOT(slotConnected()));
}

void Mod_Proxy::incomeMessage(const QByteArray &data)
{
    _data = data;
    QString _host = getHost(_data);
    _socket->connectToHost(_host, 80);

    qDebug() << Q_FUNC_INFO;
}

void Mod_Proxy::close()
{
    _socket->close();
}

void Mod_Proxy::protocolMessage()
{
    qDebug() << Q_FUNC_INFO;
    emit messageReadyRead(MOD_PROXY, _adminId, _socket->readAll());
    _socket->disconnectFromHost();
}

void Mod_Proxy::slotConnected()
{
    _socket->write(_data);

    qDebug() << Q_FUNC_INFO;
}

QString Mod_Proxy::getHost(QByteArray data)
{
    QString host;
    QString str = data.data();
    QRegExp rx("host\\s*:\\s*([A-Za-z0-9\.]+)[\n\r]+");

    if (rx.indexIn(str.toLower()) != 1)
        host = rx.cap(1);

    qDebug() << Q_FUNC_INFO;
    qDebug() << "HOST:" << host;
    qDebug() << data;

    return host;
}


}
