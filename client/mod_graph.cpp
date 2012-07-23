#include "mod_graph.h"
#include <QDebug>
#include <QTimer>
namespace delta3
{
    mod_graph::mod_graph(QObject *parent, quint16 adminId)
        : mod_abstract(parent, adminId)
    {
        const quint16 QUALITY = 20;

        _snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        _buffer.setBuffer(&_byteImage);
        _buffer.open(QIODevice::WriteOnly);
        _quality = QUALITY;
        _snapshot.save(&_buffer, "JPG", _quality);
        qDebug() << _byteImage.size();
        sendPix(_byteImage);
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
        emit messageReadyRead(MOD_GRAPH, _adminId, byteImg);
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
    #else
        qDebug() << "mod_graph::mouseClick - OS is not supported";
    #endif
    }

    void mod_graph::screentick()
    {
        _byteImage.clear();
        _snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        _buffer.open(QIODevice::WriteOnly);
        _snapshot.save(&_buffer, "JPG", _quality);
        sendPix(_byteImage);
    }
}


