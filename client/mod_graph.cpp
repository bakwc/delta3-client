#include "mod_graph.h"
#include "client.h"
#include <QDebug>
#include <QTimer>
#include <QTimerEvent>
#include <X11/extensions/XTest.h> // clicks for linux

namespace delta3
{
ModGraphics::ModGraphics(qint16 adminId, Client *client)
    : ModAbstract(MOD_GRAPHICS, adminId, client), init(false)
{
    const quint16 QUALITY = 20;

    _buffer.setBuffer(&_byteImage);
    _buffer.open(QIODevice::WriteOnly);
    _quality = QUALITY;
    _format = "PNG";

    timer.start(100, this);
}

void ModGraphics::incomeMessage(const QByteArray &data)
{
    //qDebug() << "IncomeMessage()" << data.toHex();
    switch(data[0]){
    case GMOD_INFO    :
        qDebug() << "GMOD_INFO" << (int)data[2];
        init = true;
        _quality = (quint8)data[2];
        timer.start(1000, this);
        break;
    case GMOD_IMGFULL : qDebug() << "GMOD_IMGFULL"; break;
    case GMOD_IMGDIFF : qDebug() << "GMOD_IMGDIFF"; break;
    case GMOD_KEYEV   : qDebug() << "GMOD_KEYEV"; break;
    case GMOD_MMOV    :
        qDebug() << "GMOD_MMOV";
        mouseMove(data);
        break;
    case GMOD_MCLICK  :
        qDebug() << "GMOD_MCLICK";
        mouseClick(data);
        break;
    default : break;
    }
}


void ModGraphics::mouseMove(const QByteArray& data)
{
    quint16 x = fromBytes<quint16>(data.mid(1,2));
    quint16 y = fromBytes<quint16>(data.mid(3,2));
    QCursor::setPos(x, y);

    qDebug() << Q_FUNC_INFO << x << y;
}


void ModGraphics::mouseClick(const QByteArray& data)
{
    quint16 x = fromBytes<quint16>(data.mid(1,2));
    quint16 y = fromBytes<quint16>(data.mid(3,2));
    GMCLICK click = static_cast<GMCLICK>(data[5]);
    QCursor::setPos(x, y);

    //qDebug() << Q_FUNC_INFO << x << y << click;

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

#ifdef Q_WS_X11
    Display *dspl = XOpenDisplay(NULL);

    if (!dspl) {
        qDebug("X11 display open error");
        return;
    }

    if (click & GMCLICK_CLICK) {
        clickDownFunc(click, dspl);
        clickUpFunc(click, dspl);
    } else if(click & GMCLICK_DCLICK) {
        clickDownFunc(click, dspl);
        clickUpFunc(click, dspl);
        clickDownFunc(click, dspl);
        clickUpFunc(click, dspl);
    } else if(click & GMCLICK_DOWN) {
        clickDownFunc(click, dspl);
    } else if(click & GMCLICK_UP) {
        clickUpFunc(click, dspl);
    }

    XFlush(dspl);

#endif
}


void ModGraphics::screentick()
{
    _byteImage.clear();
    _snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
    _buffer.open(QIODevice::WriteOnly);
    QSize size = QSize(_snapshot.size().width()/6, _snapshot.size().height()/6);
    _snapshot.scaled(size,Qt::KeepAspectRatio, Qt::SmoothTransformation).save(&_buffer, _format.toLocal8Bit(), _quality);

    QByteArray arr;
    arr.append(GMOD_IMGFULL);
    arr.append(_byteImage);
    client_->sendLevelTwo(MOD_GRAPHICS, adminId_, arr);
}


/* Client don't will be send image, until an admin is not initialized */
void ModGraphics::timerEvent(QTimerEvent *ev)
{
    if(init)
        screentick();
    else
        sendInform();

    ev->accept();
}


void ModGraphics::clickDownFunc(GMCLICK click, void *dspl)
{
#ifdef Q_WS_X11
    if (click & GMCLICK_LEFT) {
        XTestFakeButtonEvent((Display*)dspl, 1, 1, 200);
        qDebug() << "DOWN LEFT";
    }
    if (click & GMCLICK_MIDDLE) {
        XTestFakeButtonEvent((Display*)dspl, 2, 1, 200);
        qDebug() << "DOWN MIDDLE";
    }
    if (click & GMCLICK_RIGHT) {
        XTestFakeButtonEvent((Display*)dspl, 3, 1, 200);
        qDebug() << "DOWN RIGHT";
    }
#endif
}


void ModGraphics::clickUpFunc(GMCLICK click, void *dspl)
{
#ifdef Q_WS_X11
    if (click & GMCLICK_LEFT) {
        XTestFakeButtonEvent((Display*)dspl, 1, 0, 200);
        qDebug() << "UP LEFT";
    }
    if (click & GMCLICK_MIDDLE) {
        XTestFakeButtonEvent((Display*)dspl, 2, 0, 200);
        qDebug() << "UP MIDDLE";
    }
    if (click & GMCLICK_RIGHT) {
        XTestFakeButtonEvent((Display*)dspl, 3, 0, 200);
        qDebug() << "UP RIGHT";
    }
#endif
}


void ModGraphics::sendInform()
{
    QByteArray arr;
    arr.append(GMOD_INFO);
    arr.append(GRAPH_PROTOCOL_VERSION);
    arr.append(toBytes((qint16)(qApp->desktop()->width())));
    arr.append(toBytes((qint16)(qApp->desktop()->height())));
    client_->sendLevelTwo(mode_, adminId_, arr);
}

}


