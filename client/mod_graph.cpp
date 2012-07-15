#include "mod_graph.h"
#include <QDebug>
#include <QTimer>
namespace delta3
{
    mod_graph::mod_graph(QObject *parent, quint16 adminId_):
        QObject(parent),
        adminId(adminId_)
    {
        snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        buffer.setBuffer(&byteImage);
        buffer.open(QIODevice::WriteOnly);
        quality = 20;
        snapshot.save(&buffer, "JPG", quality);
        qDebug() << byteImage.size();
        sendPix(byteImage);
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(screentick()));
        timer->start(1000);
    }

    void mod_graph::incomeMessage(const QByteArray &data)
    {
        qDebug() << "IncomeMessage()" << data.toHex();
    }

    void mod_graph::sendPix(QByteArray &byteImg)
    {
        emit messageReadyRead(MOD_GRAPH, adminId, byteImg);
    }

    void mod_graph::close()
    {
        this->disconnect();
    }

    void mod_graph::screentick()
    {
        byteImage.clear();
        snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        snapshot.save(&buffer, "JPG", quality);
        sendPix(byteImage);
    }
}


