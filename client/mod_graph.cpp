#include "mod_graph.h"
#include <QDebug>
#include <QTimer>
#include "utils.h"
namespace delta3
{
    mod_graph::mod_graph(QObject *parent, quint16 adminId_):
        QObject(parent),
        adminId(adminId_)
    {
        snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        buffer.setBuffer(&byteImage);
        buffer.open(QIODevice::WriteOnly);
        quality = 30;
        format = "PNG";
        snapshot.save(&buffer, format.toLocal8Bit(), quality);
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
        case GMOD_INFO    : qDebug() << "GMOD_INFO"; break;
        case GMOD_IMGFULL : qDebug() << "GMOD_IMGFULL"; break;
        case GMOD_IMGDIFF : qDebug() << "GMOD_IMGDIFF"; break;
        case GMOD_KEYEV   : qDebug() << "GMOD_KEYEV"; break;
        case GMOD_MMOV    : qDebug() << "GMOD_MMOV"; break;
        case GMOD_MCLICK  : qDebug() << "MCLICK"; mouseClick(data); break;
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

    void mod_graph::mouseClick(const QByteArray& data)
    {
        quint16 x = fromBytes<quint16>(data.mid(2,2));
        quint16 y = fromBytes<quint16>(data.mid(4,2));
        QCursor::setPos(x*2, y*2);
//        if(x > 10 && y > 10){
//            x-=10;
//            y-=10;
//        }
    #ifdef Q_OS_WIN
        INPUT    Input={0};
        if(data[1] == Qt::LeftButton){

        // +mouse1
            Input.type        = INPUT_MOUSE;
            Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
            ::SendInput(1, &Input, sizeof(INPUT));
        }

        // -mouse1
     //   ::ZeroMemory(&Input, sizeof(INPUT));
        if(data[1] == Qt::LeftButton + 20){
            Input.type        = INPUT_MOUSE;
            Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
            ::SendInput(1, &Input,sizeof(INPUT));
        }
    #endif
    }

    void mod_graph::screentick()
    {
        byteImage.clear();
        snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        buffer.open(QIODevice::WriteOnly);
        snapshot.save(&buffer, format.toLocal8Bit(), quality);
        sendPix(byteImage);
    }
}


