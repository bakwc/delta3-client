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


        void parsePing();
        void parseResponse();
        void parseProtoTwo(qint16 from, const QByteArray &data);

private:
        // В эти мапы добавляются протоколы
        // при активации их любым их админов
        // Для быстрого доступа по айди.
		QMap < qint16, ModTelnet* > test1;
		QMap < qint16, ModGraphics* > test2;

        // Послать дату в формате согласным с протоколом
        void sendData2(qint16 adminId, const QByteArray &data);

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


        // Обработка сообщений на протоколы 3 уровня
        void parseProtocolsMessages(qint16 adminId, const QByteArray &data);

        // Обработка запросов листа.
        // Шлет на сервер доступные протоколы
        void sendAvailableProtocols(qint16 adminId);

        // Обработка сообщений активации
        // и деактивации протоколов 3 уровня
        void activateDeactivateProtocol(Cspyp2Command turn, qint16 adminId, ProtocolMode proto);

        // Список доступных протоколов
        QVector<ProtocolMode> availableProtocols;

        void sendData1(QByteArray &data);


    public slots:


        void sendData3(ProtocolMode mode, qint16 adminId, QByteArray data);

        // Запускаеться при коннекте к мастер-серверу, пока
        // генериться простой md5 (из QTime::currentTime().msec())
        void onConnect();

        // Запускаеться при дисконнекте
        void onDisconnect();

        // Запускаеться при входящем сообщении
        // Сортирует сообщения
        // и кидает необходимому обработчику
        void onDataReceived();

	private:
		QString getOS();
		QByteArray buf_;
		QTcpSocket * socket;
		quint16 port;
		QHostAddress server;

		QMap<ProtocolMode, QMap<qint16, ModAbstract*> > mods_;
    };
}
