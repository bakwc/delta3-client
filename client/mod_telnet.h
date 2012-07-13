#pragma once
#include <QObject>
#include <QProcess>
class mod_telnet : public QObject
{
    Q_OBJECT

public:
    explicit mod_telnet(QObject *parent = 0, quint32 adminId = 0);
    QProcess * protocol;
    void incomeMessage(const QByteArray &data);
    quint32 adminId;
    void close();
public slots:
    void protocolMessage();

signals:
    void messageReadyRead(qint32, QByteArray&);
};
