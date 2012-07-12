#include "Client.h"
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

    parseFirstProtocol(clientId, data);
    sendAvailableProtocols(clientId, data);
    activateDeactivateProtocol(clientId, data);

}

void Client::parseFirstProtocol(qint32 adminId, const QByteArray& data)
{
    // Не шарю в решулярках, наверно что-то не так,
    // но худо-бедно работает
    QRegExp rx = QRegExp("1:(\\w+):(.*)");
    if (rx.indexIn(data)==-1)
        return;

    qDebug() << "parseFirstProtocol()";

    // Если команда протоколу правильная
    // и протокол запущен
    // отправить запрос в первый протокол
    // в данном случае в консоль винды
    if((rx.captureCount() != -1) && telnetProcess->Running)
        telnetProcess->write(qPrintable(QString("%1\n")
                           .arg(rx.cap(2).left(rx.cap(1).toInt()))));
}

void Client::sendAvailableProtocols(qint32 adminId, const QByteArray& data)
{
    // Крайне криво на мой вгляд.
    // Нужно упростить.
    QRegExp rx = QRegExp("^l:");
    if (rx.indexIn(data)==-1)
        return;

    QString protocols = "m:";
    for(short i = 0; i < availableProtocols.size(); ++i){
        protocols.append(QString::number(availableProtocols.at(i)));
        if(i < availableProtocols.size()-1)
            protocols.append(";");
    }
    protocols.append(":");
    qDebug() << protocols;
    sendData(adminId, qPrintable(protocols));
}

void Client::activateDeactivateProtocol(qint32 adminId, const QByteArray& incoming)
{
    QRegExp rx = QRegExp("(\\w):(\\d+):");
    if (rx.indexIn(incoming)==-1)
        return;

    ActivationMode mode;
    if (rx.cap(1) == "a") mode=ACT_ACTIVATE;
    else if (rx.cap(1) == "d") mode=ACT_DEACTIVATE;
    else
        return; // If not activate or deactivate - exit

     qDebug() << "activateDeactivateProtocol()";

    ProtocolMode proto = (ProtocolMode)rx.cap(2).toInt();
    this->adminId = adminId;

    if (mode == ACT_ACTIVATE){
        if (proto == MOD_TELNET){
            telnetProcess = new QProcess(this);
#ifdef Q_WS_X11
            firstprotocol->start("bash");
#endif

#ifdef Q_WS_MAC
            firstprotocol->start("bin/bash");
#endif

#ifdef Q_WS_QWS
            firstprotocol->start("bash");
#endif

#ifdef Q_WS_WIN
            telnetProcess->start("cmd");
#endif
            connect(telnetProcess,
                    SIGNAL(readyReadStandardOutput()),
                    this,
                    SLOT(slotDataOnStdout())
                    );
        }
    }
    else if (mode == ACT_DEACTIVATE) {
        telnetProcess->disconnect(this);
        telnetProcess->close();
    }

}

void Client::sendData(qint32 adminId, const QByteArray& data)
{
    QByteArray gogo = (QString("t:%1:%2:").arg(adminId).arg(data.size()))
            .toUtf8() + data;
    socket->write(gogo);
}

void Client::slotDataOnStdout(){
    QString output = QString::fromLocal8Bit(telnetProcess->readAllStandardOutput());
    QByteArray data=(QString("1:%1:%2:").arg(output.size()).arg(output)).toUtf8();
    sendData(this->adminId, data);
}
