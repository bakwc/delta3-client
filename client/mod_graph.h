#pragma once
#include <QObject>
#include <QPixmap>
#include <QBuffer>
#include <QApplication>
#include <QDesktopWidget>
class mod_graph : public QObject
{
    Q_OBJECT

public:
    explicit mod_graph(QObject *parent = 0, quint32 adminId = 0);
    QPixmap snapshot;
    QBuffer buffer;
    QByteArray byteImage;
    void incomeMessage(const QByteArray &data);
    quint32 adminId;
    void sendPix(const QByteArray &byteImg);
    void close();
public slots:
    void protocolMessage();

signals:
    void messageReadyRead(qint32, QByteArray&);
};
