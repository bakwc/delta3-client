#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QCoreApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <QProcess>
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

    // Обработка t: сообщений не начинал
    void parseSomeInformation(QString incoming);

    // Обработка 1: сообщений.
    // Сделал только консоль cmd
    void parseFirstProtocol(QString incoming);

    // Обработка l: сообшений.
    // Шлет на сервер доступные протоколы
    void sendAvailableProtocols();

    // Обработка a: и d: сообщений.
    // a:1: и d:1: работают,
    // но очень слабая реализация
    void activateDeactivateProtocol(QString incoming);

    // Через этот процесс работает 1 протокол
    QProcess * firstprotocol;

    // Список доступных протоколов
    QVector<int> availableProtocols;
    
public slots:

    // Запускаеться при коннекте к мастер-серверу
    // На данный момент генериться простой md5
    // и обрезается до 9 октобайт (так в протоколе)
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

#endif // CLIENT_H
