#include "client.h"
#include "mod_telnet.h"
#include "mod_graph.h"
#include <iostream>
#include <QCryptographicHash>
#include <QTime>
#include <QtGlobal>
#include "utils.h"

namespace delta3
{
    Client::Client(QObject *parent) :
        QObject(parent),
        port(1235),
        server(QHostAddress::LocalHost)
    {
        // Добавялем первый протокол
        availableProtocols.push_back(MOD_TELNET);

        // Добавляем второй протокол
		availableProtocols.push_back(MOD_GRAPHICS);
        qDebug() << server << port;
        socket = new QTcpSocket(this);

        // Соединение с мастер-сервером
        socket->connectToHost(server, port);

        // Подключение всех необходимых сигналов
        // Исправить : не подключать если нет коннекта
        connect(socket, SIGNAL(connected()), this, SLOT(onConnect()));
        connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
        connect(socket, SIGNAL(readyRead()), this, SLOT(onDataReceived()));

        // Ожидание 3 секунды,
        // на случай медленного инета
        if(socket->waitForConnected(3000))
            qDebug() << "Connected to " << server.toString() << port;
        else
            qDebug() << "Unable to connect";


    }

    QString Client::getOS()
        {

            static QString osVersion;
                if(osVersion.isEmpty())
                {
            #if defined(Q_OS_LINUX)
                #if defined(Q_WS_X11)
                    osVersion = QLatin1String("Linux(X11)");
                #else
                    osVersion = QLatin1String("Linux(unknown)");
                #endif
            #elif defined(Q_OS_MAC)
                    switch(QSysInfo::MacintoshVersion)
                    {
                        case QSysInfo::MV_SNOWLEOPARD:
                            osVersion = QLatin1String("MacOS 10.6(SnowLeopard");
                        case QSysInfo::MV_LEOPARD:
                            osVersion = QLatin1String("MacOS 10.5(Leopard)");
                            break;
                        case QSysInfo::MV_TIGER:
                            osVersion = QLatin1String("MacOS 10.4(Tiger)");
                            break;
                        case QSysInfo::MV_PANTHER:
                            osVersion = QLatin1String("MacOS 10.3(Panther)");
                            break;
                        case QSysInfo::MV_JAGUAR:
                            osVersion = QLatin1String("MacOS 10.2(Jaguar)");
                            break;
                        case QSysInfo::MV_PUMA:
                            osVersion = QLatin1String("MacOS 10.1(Puma)");
                            break;
                        case QSysInfo::MV_CHEETAH:
                            osVersion = QLatin1String("MacOS 10.0(Cheetah)");
                            break;
                        case QSysInfo::MV_9:
                            osVersion = QLatin1String("MacOS 9");
                            break;

                        case QSysInfo::MV_Unknown:
                        default:
                            osVersion = QLatin1String("MacOS(unknown)");
                            break;
                    }
            #elif defined(Q_OS_WIN)
                    switch(QSysInfo::WindowsVersion)
                    {
                        case QSysInfo::WV_WINDOWS7:
                            osVersion = QLatin1String("Windows 7");
                        break;
                        case QSysInfo::WV_CE_6:
                            osVersion = QLatin1String("Windows CE 6.x");
                            break;
                        case QSysInfo::WV_CE_5:
                            osVersion = QLatin1String("Windows CE 5.x");
                            break;
                        case QSysInfo::WV_CENET:
                            osVersion = QLatin1String("Windows CE .NET");
                            break;
                        case QSysInfo::WV_CE:
                            osVersion = QLatin1String("Windows CE");
                            break;

                        case QSysInfo::WV_VISTA:
                            osVersion = QLatin1String("Windows Vista");
                            break;
                        case QSysInfo::WV_2003:
                            osVersion = QLatin1String("Windows Server 2003");
                            break;
                        case QSysInfo::WV_XP:
                            osVersion = QLatin1String("Windows XP");
                            break;
                        case QSysInfo::WV_2000:
                            osVersion = QLatin1String("Windows 2000");
                            break;
                        case QSysInfo::WV_NT:
                            osVersion = QLatin1String("Windows NT");
                            break;

                        case QSysInfo::WV_Me:
                            osVersion = QLatin1String("Windows Me");
                            break;
                        case QSysInfo::WV_98:
                            osVersion = QLatin1String("Windows 98");
                            break;
                        case QSysInfo::WV_95:
                            osVersion = QLatin1String("Windows 95");
                            break;
                        case QSysInfo::WV_32s:
                            osVersion = QLatin1String("Windows 3.1");
                            break;

                        default:
                            osVersion = QLatin1String("Windows(unknown)");
                            break;
                    }
            #else
                    return QLatin1String("Unknown");
            #endif
                }

                return osVersion;
        }

    void Client::onConnect()
    {
        QString time = QString("%1").arg( QTime::currentTime().msec() );
        QByteArray md5hash = QCryptographicHash::hash(time.toLocal8Bit(), QCryptographicHash::Md5);
        QByteArray hello;
        hello.append(CSPYP1_PROTOCOL_ID);
        hello.append(CSPYP1_PROTOCOL_VERSION);
        hello.append(CMD1_AUTH);
        hello.append(md5hash);

        hello.append( toBytes(getOS(), 20), 20 );
        hello.append( toBytes("desktop", 20), 20 );

        //hello = hello.leftJustified( 59, 0 );
		qDebug() << "onConnect()" << hello.toHex() << "Command size:" << hello.size();

        socket->write(hello);
    }

    void Client::onDisconnect()
    {
        qDebug() << "The connection to the server has been lost";
    }

    void Client::onDataReceived()
    {
        qDebug() << "onDataReceived():";
        buf_ += socket->readAll();
        qDebug() << buf_.toHex();
        if (buf_.size() < 3) return; // if we don't read header

		qDebug() << "    ID:" << getProtoId(buf_) << " Version" << getProtoVersion(buf_);

        if (    getProtoId(buf_)       != CSPYP1_PROTOCOL_ID ||
                getProtoVersion(buf_) != CSPYP1_PROTOCOL_VERSION)
        {
            // wrong packet - disconnecting client
            qDebug() << "PROTOCOL ERROR!";
            //this->disconnectFromHost();
            return;
        }

		qDebug() << "    buf size" << buf_.size() << " cmd:" << getCommand(buf_);

        switch (getCommand(buf_))
        {

        case CMD1_TRANSMIT:
            qDebug() << "transmit";
            parseResponse();
            break;
        case CMD1_PING:
            qDebug() << "ping";
            parsePing();
            break;
        default:
            break;
        }
    }

    void Client::parsePing()
    {
		//qDebug() << "Ping received!";
        if (buf_.size() < 3) // TODO: remove magic number
            return;     // not all data avaliable

        QByteArray cmd;
        cmd.append(CSPYP1_PROTOCOL_ID);
        cmd.append(CSPYP1_PROTOCOL_VERSION);
        cmd.append(CMD1_PING);
        socket->write(cmd);

		//qDebug() << "Ping parsed and answere!";

        buf_ = buf_.right(buf_.size() - 3);
        if (buf_.size() > 0)
            onDataReceived();   // If something in buffer - parse again
    }

    void Client::parseResponse()
    {
        qDebug() << "parseResponse()";
        qDebug() << buf_.toHex();

        if (buf_.size() < 9) // TODO: remove magic number
            return;     // not all data avaliable

        if (buf_.size() < getPacketLength(buf_) + 9) // TODO: remove magic number
            return; // not all data avaliable

        qint16 from = getClientId(buf_);

        QByteArray cmd = getPacketData(buf_);

        parseProtoTwo(from, cmd);
        buf_ = buf_.right(buf_.size() - (getPacketLength(buf_) + 9));
        if (buf_.size() > 0)
            onDataReceived();   // If something in buffer - parse again

        return;
    }

    void Client::parseProtoTwo(qint16 from, const QByteArray &data)
    {
        qDebug() << "parseProtoTwo():" << data.size();
        qDebug() << data.toHex();

        if (    getProtoId(data)     != CSPYP2_PROTOCOL_ID ||
                getProtoVersion(data)!= CSPYP2_PROTOCOL_VERSION)
        {
            // wrong packet - disconnecting client
            qDebug() << "PROTOCOL ERROR!";
            //this->disconnectFromHost();
            return;
        }

        switch (getCommand2(data))
        {
        case CMD2_LIST:
			qDebug() << "list";
            sendAvailableProtocols(from);
            break;
        case CMD2_TRANSMIT:
			qDebug() << "CMD2_TRANSMIT";
            parseProtocolsMessages(from, data);
            break;
        case CMD2_ACTIVATE:
			//qDebug() << "Activation";
            activateDeactivateProtocol(CMD2_ACTIVATE, from, (ProtocolMode) data[3]);
            break;
        case CMD2_DEACTIVATE:
			//qDebug() << "Deactivation";
            activateDeactivateProtocol(CMD2_DEACTIVATE, from,(ProtocolMode) data[3]);
            break;

        default:
            break;
        }
    }

    void Client::parseProtocolsMessages(qint16 adminId, const QByteArray& data)
    {
        qDebug() << data << getMode(data);

        mods_[getMode(data)][adminId]->incomeMessage(data.mid(8));
    }

    void Client::sendAvailableProtocols(qint16 adminId)
    {
        QByteArray protocolList;
        protocolList.append(CMD2_MODES);
        protocolList.append(toBytes(availableProtocols.size()));

        for(short i = 0; i < availableProtocols.size(); ++i){
            protocolList.append(availableProtocols.at(i));
        }
        qDebug() << "sendAvailableProtocols\n";
        sendData2(adminId, protocolList);
    }

    void Client::activateDeactivateProtocol(Cspyp2Command turn, qint16 adminId, ProtocolMode proto)
    {
        if (turn == CMD2_ACTIVATE){

            switch(proto){

            case MOD_TELNET :
                mods_[proto].insert(adminId, new ModTelnet(adminId, this));
                break;

            case MOD_GRAPHICS :
                mods_[proto].insert(adminId, new ModGraphics(adminId, this));
                break;

            default:
                break;
            }

        } else if (turn == CMD2_DEACTIVATE) {
                delete mods_[proto][adminId];
                mods_[proto].remove(adminId);
        }
    }

    void Client::sendData3(ProtocolMode mode, qint16 adminId, QByteArray data)
    {
        QByteArray buf;
        buf.append(CSPYP2_PROTOCOL_ID);
        buf.append(CSPYP2_PROTOCOL_VERSION);
        buf.append(CMD2_TRANSMIT);
        buf.append(mode);
        buf.append(toBytes(data.size()));
        buf.append(data);
        sendData2(adminId, buf);
    }

    void Client::sendData2(qint16 adminId, const QByteArray &data)
    {
        QByteArray dataToSend;
        dataToSend.append(CSPYP1_PROTOCOL_ID);
        dataToSend.append(CSPYP1_PROTOCOL_VERSION);
        dataToSend.append(CMD1_TRANSMIT);
        dataToSend.append(toBytes(adminId));
        dataToSend.append(toBytes(data.size()));
        dataToSend.append(data);
        socket->write(dataToSend);
    }
}
