#pragma once
#include <QObject>
#include <QCoreApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <QProcess>
#include <QByteArray>
#include "defines.h"
#include "mod_telnet.h"
#include "mod_graph.h"
#include <QMap>
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    void setAddress(const QString &addr );
    void setPort( quint16 port );

private:
    QTcpSocket * socket;
    quint16 port;
    QHostAddress server;
    QMap < qint32, mod_telnet* > test1;
    QMap < qint32, mod_graph* > test2;

    // Обработка 1: сообщений.
    // Сделал только консоль cmd
    void parseProtocolsMessages(qint32 adminId, const QByteArray &data);

    // Обработка l: сообшений.
    // Шлет на сервер доступные протоколы
    void sendAvailableProtocols(qint32 adminId, const QByteArray &data);

    // Обработка a: и d: сообщений.
    // a:1: и d:1: работают,
    // но очень слабая реализация
    void activateDeactivateProtocol(qint32 adminId, const QByteArray &incoming);

    // Список доступных протоколов
    QVector<ProtocolMode> availableProtocols;

    
public slots:

    // Послать дату в формате согласным с протоколом
    void sendData(qint32 adminId, QByteArray &data);

    // Запускаеться при коннекте к мастер-серверу, пока
    // генериться простой md5 (из QTime::currentTime().msec())
    void onConnect();

    // Запускаеться при дисконнекте
    void onDisconnect();

    // Запускаеться при входящем сообщении
    // Сортирует сообщения
    // и кидает необходимому обработчику
    void sortIncomingData();

};
