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
#include "utils.h"
namespace delta3
{

    class Client : public QObject
    {
        Q_OBJECT
    public:
        explicit Client(QObject *parent = 0);
        void setAddress(const QString &addr);
        void setPort(quint16 port);

        void sendData3(ProtocolMode mode, qint16 adminId, const QByteArray &data);
        void parsePing();
        void parseResponse();
        void parseProtoTwo(qint16 from, const QByteArray &data);
    private:
        QByteArray buf_;
        QTcpSocket * socket;
        quint16 port;
        QHostAddress server;
        QMap < qint16, mod_telnet* > test1;
        QMap < qint16, mod_graph* > test2;

        qint8 getProtoId(const QByteArray& buffer)
        {
            return (qint8)(buffer[0]);
        }

        Cspyp1Command getCommand(const QByteArray& buffer)
        {
            return (Cspyp1Command)(buffer[2]);
        }

        qint8 getProtoVersion(const QByteArray& buffer)
        {
            return (qint8)(buffer[1]);
        }

        qint16 getClientId(const QByteArray& data)
        {
            QByteArray clientId = data.mid(3, 2);
            return fromBytes<qint16>(clientId);
        }

        QByteArray getPacketData(const QByteArray& data)
        {
            return data.mid(9, getPacketLength(data));
        }

        qint32 getPacketLength(const QByteArray& data)
        {
            QByteArray clientNum = data.mid(5, 4);
            return fromBytes<quint32>(clientNum);
        }

        ProtocolMode getMode(const QByteArray& data)
        {
            return (ProtocolMode)data[3];
        }

        Cspyp2Command getCommand2(const QByteArray& data)
        {
            return (Cspyp2Command)data[2];
        }


        // Обработка 1: сообщений.
        // Сделал только консоль cmd
        void parseProtocolsMessages(qint16 adminId, const QByteArray &data);

        // Обработка l: сообшений.
        // Шлет на сервер доступные протоколы
        void sendAvailableProtocols(qint16 adminId);

        // Обработка a: и d: сообщений.
        // a:1: и d:1: работают,
        // но очень слабая реализация
        void activateDeactivateProtocol(Cspyp2Command turn, qint16 adminId, ProtocolMode proto);

        // Список доступных протоколов
        QVector<ProtocolMode> availableProtocols;

        void sendData1(QByteArray &data);


    public slots:

        // Послать дату в формате согласным с протоколом
        void sendData2(qint16 adminId, const QByteArray &data);

        // Запускаеться при коннекте к мастер-серверу, пока
        // генериться простой md5 (из QTime::currentTime().msec())
        void onConnect();

        // Запускаеться при дисконнекте
        void onDisconnect();

        // Запускаеться при входящем сообщении
        // Сортирует сообщения
        // и кидает необходимому обработчику
        void onDataReceived();
    };
}
