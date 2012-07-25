#include "mod_graph.h"
#include <QDebug>
#include <QTimer>
namespace delta3
{
ModGraphics::ModGraphics(qint16 adminId, Client *client)
		: ModAbstract(MOD_GRAPHICS, adminId, client)
    {
        const quint16 QUALITY = 20;

		//_snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        _buffer.setBuffer(&_byteImage);
        _buffer.open(QIODevice::WriteOnly);
        _quality = QUALITY;
        _format = "PNG";
//        qDebug() << _byteImage.size();
//        QSize size = QSize(_snapshot.size().width()/2, _snapshot.size().height()/2);
//        _snapshot.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(&_buffer, _format.toLocal8Bit(), _quality);
//        _snapshot.save(&_buffer, _format.toLocal8Bit(), _quality);
//        sendPix(_byteImage);

		screentick();

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(screentick()));
        timer->start(1000);
    }

	void ModGraphics::incomeMessage(const QByteArray &data)
    {
        qDebug() << "IncomeMessage()" << data.toHex();
        switch(data[0]){
        case GMOD_INFO    : qDebug() << "GMOD_INFO"; break;
        case GMOD_IMGFULL : qDebug() << "GMOD_IMGFULL"; break;
        case GMOD_IMGDIFF : qDebug() << "GMOD_IMGDIFF"; break;
        case GMOD_KEYEV   : qDebug() << "GMOD_KEYEV"; break;
        case GMOD_MMOV    : qDebug() << "GMOD_MMOV"; mouseMove(data); break;
        case GMOD_MCLICK  : qDebug() << "MCLICK"; mouseClick(data); break;
        default : break;
        }
    }


	void ModGraphics::mouseMove(const QByteArray& data){
        quint16 x = fromBytes<quint16>(data.mid(1,2));
        quint16 y = fromBytes<quint16>(data.mid(3,2));
        QCursor::setPos(x*2, y*2);
    }

	void ModGraphics::mouseClick(const QByteArray& data)
    {
        quint16 x = fromBytes<quint16>(data.mid(1,2));
        quint16 y = fromBytes<quint16>(data.mid(3,2));
        QCursor::setPos(x*2, y*2);

    #ifdef Q_OS_WIN

        quint8 mCode;
        switch(data[5]){
        case Qt::LeftButton        : mCode = MOUSEEVENTF_LEFTDOWN; break;
        case Qt::RightButton       : mCode = MOUSEEVENTF_RIGHTDOWN; break;
        case Qt::MidButton         : mCode = MOUSEEVENTF_MIDDLEDOWN;break;
        case Qt::LeftButton  + 20  : mCode = MOUSEEVENTF_LEFTUP; break;
        case Qt::RightButton + 20  : mCode = MOUSEEVENTF_RIGHTUP; break;
        case Qt::MidButton   + 20  : mCode = MOUSEEVENTF_MIDDLEUP; break;
        default                    : mCode = 0xFF;
        }

        if(mCode != 0xFF){
            INPUT    Input={0};
            Input.type        = INPUT_MOUSE;
            Input.mi.dwFlags  = mCode;
            ::SendInput(1, &Input, sizeof(INPUT));
        }


    #endif
    }

	void ModGraphics::screentick()
    {
        _byteImage.clear();
        _snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
        _buffer.open(QIODevice::WriteOnly);
        QSize size = QSize(_snapshot.size().width()/2, _snapshot.size().height()/2);
        _snapshot.scaled(size,Qt::KeepAspectRatio, Qt::SmoothTransformation).save(&_buffer, _format.toLocal8Bit(), _quality);
		//sendPix(_byteImage);

		emit messageReadyRead(MOD_GRAPHICS, _adminId, _byteImage);

    }
}


