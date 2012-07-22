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
        switch(data[0]){
        case GMOD_IMGFULL    : qDebug() << "GMOD_IMGFULL"; break;
        case GMOD_IMGHALF    : qDebug() << "GMOD_IMGHALF"; break;
        case GMOD_IMGSTART   : qDebug() << "GMOD_IMGSTART"; break;
        case GMOD_IMGMID     : qDebug() << "GMOD_IMGMID"; break;
        case GMOD_IMGEND     : qDebug() << "GMOD_IMGEND"; break;
        case GMOD_KEYPRESSED : qDebug() << "GMOD_KEYPRESSED"; break;
        case GMOD_MMOVE      : qDebug() << "GMOD_MMOVE"; break;
        case GMOD_MLEFT      : qDebug() << "GMOD_MLEFT"; break;
        case GMOD_MRIGHT     : qDebug() << "GMOD_MRIGHT"; break;
        case GMOD_MMID       : qDebug() << "GMOD_MMID"; break;
        default : break;
        }
    }

    void mod_graph::sendPix(QByteArray &byteImg)
    {
        emit messageReadyRead(MOD_GRAPH, adminId, byteImg);
    }

    void mod_graph::close()
    {
        this->disconnect();
    }

    void mod_graph::mouseClick(GraphMode key, quint16 x, quint16 y)
    {
    #ifdef Q_OS_WIN
        INPUT    Input={0};

        // +mouse1
        Input.type        = INPUT_MOUSE;
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
        ::SendInput(1, &Input, sizeof(INPUT));

        // -mouse1
        ::ZeroMemory(&Input, sizeof(INPUT));
        Input.type        = INPUT_MOUSE;
        Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
        ::SendInput(1, &Input,sizeof(INPUT));
    #endif
    }

    void mod_graph::screentick()
    {
        byteImage.clear();
        snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        buffer.open(QIODevice::WriteOnly);
        snapshot.save(&buffer, "JPG", quality);
        sendPix(byteImage);
    }
}


