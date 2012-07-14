#include "mod_graph.h"
#include <QDebug>

namespace delta3
{
    mod_graph::mod_graph(QObject *parent, quint32 adminId_):
        QObject(parent),
        adminId(adminId_)
    {
        snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        buffer.setBuffer(&byteImage);
        buffer.open(QIODevice::WriteOnly);
        snapshot.save(&buffer, "JPG", 20);
        qDebug() << byteImage.size();
        sendPix(byteImage);
    }

    void mod_graph::incomeMessage(const QByteArray &data)
    {
    }

    void mod_graph::sendPix(const QByteArray &byteImg)
    {

    }

    void mod_graph::protocolMessage()
    {
    }

    void mod_graph::close()
    {
        this->disconnect();
    }
}


