#pragma once
#include <QObject>
#include <QCoreApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <QProcess>
#include <QByteArray>

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

    // Обработка 1: сообщений.
    // Сделал только консоль cmd
    void parseFirstProtocol(qint32 adminId, QString incoming);

    // Обработка l: сообшений.
    // Шлет на сервер доступные протоколы
    void sendAvailableProtocols(qint32 adminId);

    // Обработка a: и d: сообщений.
    // a:1: и d:1: работают,
    // но очень слабая реализация
    void activateDeactivateProtocol(qint32 adminId, QString incoming);

    // Через этот процесс работает 1 протокол
    QProcess * firstprotocol;

    // Список доступных протоколов
    QVector<int> availableProtocols;

    // Послать дату в формате согласным с протоколом
    void sendData(qint32 adminId, QString data);
    
    // Здесь хранится айди админа, который активировал
    // один из протоколов 3 уровня. TODO: Исправить на N админов.
    qint32 adminId;
    
public slots:

    // Запускаеться при коннекте к мастер-серверу, пока
    // генериться простой md5 (из QTime::currentTime().msec())
    void onConnect();

    // Запускаеться при дисконнекте
    void onDisconnect();

    // Запускаеться при входящем сообщении
    // Сортирует сообщения
    // и кидает необходимому обработчику
    void sortIncomingData();

    // Запускаеться при выводе 1 протокола
    void slotDataOnStdout();
};
