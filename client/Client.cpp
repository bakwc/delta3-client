#include "Client.h"
#include "mod_telnet.h"
#include "mod_graph.h"
#include <iostream>
#include <QCryptographicHash>
#include <QTime>
#include <QRegExp>
Client::Client(QObject *parent) :
    QObject(parent),
    port(1235),
    server(QHostAddress::LocalHost)
{
    // Добавялем первый протокол
    availableProtocols.push_back(MOD_TELNET);
   // availableProtocols.push_back(MOD_GRAPH);
    qDebug() << server << port;
    socket = new QTcpSocket(this);

    // Соединение с мастер-сервером
    socket->connectToHost(server, port);

    // Подключение всех необходимых сигналов
    // Исправить : не подключать если нет коннекта
    connect(socket, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(sortIncomingData()));

    // Ожидание 3 секунды,
    // на случай медленного инета
    if(socket->waitForConnected(3000))
        qDebug() << "Connected to " << server.toString() << port;
    else
        qDebug() << "Unable to connect";

}

void Client::onConnect()
{
    QString time = QString("%1").arg( QTime::currentTime().msec() );
    QString md5hash = QString(QCryptographicHash::hash((qPrintable(time)), QCryptographicHash::Md5).toHex());
    socket->write(qPrintable(QString("cspycli:1:%1:").arg(md5hash)));
}

void Client::onDisconnect()
{
    qDebug() << "The connection to the server has been lost";
}

void Client::sortIncomingData()
{
    QRegExp rx("f:(\\d+):(\\d+):(.*)");
    QByteArray incoming = socket->readAll();
    qDebug() << QString::fromUtf8(incoming);

    // Проверяем запрос
    if(rx.indexIn(incoming) == -1)
        return;

    qDebug() << "sortIncomingData()";

    // Берём в data дату из запроса
    QByteArray data = rx.cap(3).toUtf8();
    qint32 clientId = rx.cap(1).toInt();

    qDebug() << QString::fromUtf8(data);

    parseProtocolsMessages(clientId, data);
    sendAvailableProtocols(clientId, data);
    activateDeactivateProtocol(clientId, data);

}

void Client::parseProtocolsMessages(qint32 adminId, const QByteArray& data)
{
    QRegExp rx = QRegExp("(\\d+):(\\d):(.*)");
    if(rx.indexIn(data) == -1)
        return;

    switch(rx.cap(1).toInt()){
    case MOD_TELNET : test1.find(adminId).value()->
                incomeMessage(rx.cap(3).left(rx.cap(2).toInt()).toLocal8Bit());
        break;

    case MOD_GRAPH  : test2.find(adminId).value()->
                incomeMessage(rx.cap(3).left(rx.cap(2).toInt()).toLocal8Bit());
        break;
    }
}

void Client::sendAvailableProtocols(qint32 adminId, const QByteArray& data)
{
    // Крайне криво на мой вгляд.
    // Нужно упростить.
    QRegExp rx = QRegExp("^l:");
    if (rx.indexIn(data) == -1)
        return;

    QString protocols = "m:";
    for(short i = 0; i < availableProtocols.size(); ++i){
        protocols.append(QString::number(availableProtocols.at(i)));
        if(i < availableProtocols.size() - 1)
            protocols.append(";");
    }
    protocols.append(":");
    qDebug() << protocols;
    sendData(adminId, protocols.toLocal8Bit());
}

void Client::activateDeactivateProtocol(qint32 adminId, const QByteArray& incoming)
{
    QRegExp rx = QRegExp("(\\w):(\\d+):");
    if (rx.indexIn(incoming)==-1)
        return;

    ActivationMode mode;
    if (rx.cap(1) == "a") mode = ACT_ACTIVATE;
    else if (rx.cap(1) == "d") mode = ACT_DEACTIVATE;
    else
        return; // If not activate or deactivate - exit

    qDebug() << "activateDeactivateProtocol()";

    ProtocolMode proto = (ProtocolMode)rx.cap(2).toInt();

    if (mode == ACT_ACTIVATE){
        switch(proto){

        case MOD_TELNET :{
            mod_telnet * newone = new mod_telnet(this, adminId);
            connect(newone,
                    SIGNAL(messageReadyRead(qint32, QByteArray&)),
                    this,
                    SLOT(sendData(qint32, QByteArray&))
                    );
            test1.insert(adminId, newone);
            break;
        }

        case MOD_GRAPH :{
            mod_graph * newone = new mod_graph(this, adminId);
            connect(newone,
                    SIGNAL(messageReadyRead(qint32,QByteArray&)),
                    this,
                    SLOT(sendData(qint32,QByteArray&)));
            test2.insert(adminId, newone);
            break;
        }
        }

    }
    else if (mode == ACT_DEACTIVATE) {
        switch(proto){
        case MOD_TELNET :
            test1.find(adminId).value()->close();
            delete test1.find(adminId).value();
            test1.remove(adminId);
            break;

        case MOD_GRAPH :
            test2.find(adminId).value()->close();
            delete test2.find(adminId).value();
            test2.remove(adminId);
            break;
        }


    }

}

void Client::sendData(qint32 adminId, QByteArray &data)
{
    QByteArray gogo = (QString("t:%1:%2:").arg(adminId).arg(data.size()))
            .toUtf8() + data;
    socket->write(gogo);
}
