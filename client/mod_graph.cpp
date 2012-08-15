#include "mod_graph.h"
#include "client.h"
#include <QDebug>
#include <QTimer>
#include <QTimerEvent>
#ifdef Q_WS_X11
#include <X11/extensions/XTest.h> // clicks for linux
#endif

namespace delta3
{

quint8 ModGraphics::IMGSIZE = 64; // Fucking cpp syntax

ModGraphics::ModGraphics(qint16 adminId, Client *client)
    : ModAbstract(MOD_GRAPHICS, adminId, client), init(false)
{
    const quint16 QUALITY = 20;

    _quality = QUALITY;
    _format = "png";

    timer.start(100, this);
}

void ModGraphics::incomeMessage(const QByteArray &data)
{
    switch(data[0]){
    case GMOD_INFO    :
        init = true;
        _quality = (quint8)data[2];
        timer.start(50, this);
        break;
    case GMOD_IMGFULL : qDebug() << "GMOD_IMGFULL"; break;
    case GMOD_IMGDIFF : qDebug() << "GMOD_IMGDIFF"; break;
    case GMOD_KEYEV   : qDebug() << "GMOD_KEYEV"; break;
    case GMOD_MMOV    :
        mouseMove(data);
        break;
    case GMOD_MCLICK  :
        mouseClick(data);
        break;
    default : break;
    }
}


void ModGraphics::mouseMove(const QByteArray& data)
{
    quint16 x = fromBytes<quint16>(data.mid(1,2));
    quint16 y = fromBytes<quint16>(data.mid(3,2));
    QCursor::setPos(x, y);}


void ModGraphics::mouseClick(const QByteArray& data)
{
    quint16 x = fromBytes<quint16>(data.mid(1,2));
    quint16 y = fromBytes<quint16>(data.mid(3,2));
    GMCLICK click = static_cast<GMCLICK>(data[5]);
    QCursor::setPos(x, y);

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
    //_byteImage.clear();
    //_snapshot = QPixmap::grabWindow(QApplication::desktop()->winId());
    _snap1 = QPixmap::grabWindow(QApplication::desktop()->winId()).toImage();
    //_buffer.open(QIODevice::WriteOnly);
    QSize size = QSize(_snap1.size().width()/2, _snap1.size().height()/2);
    _snap1 = _snap1.scaled(size,Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QSize iSize(0, 0);
    int         imgStrctCount = 0;
    QByteArray  imgStruct;

    while (iSize.width() < size.width()) {
        while (iSize.height() < size.height()) {
            int x = size.width() - iSize.width();
            int y = size.height() - iSize.height();

            if (x >= IMGSIZE && y >= IMGSIZE) {
                const QImage &img1 = _snap1.copy(iSize.width(), iSize.height(), IMGSIZE, IMGSIZE);
                const QImage &img2 = _snap2.copy(iSize.width(), iSize.height(), IMGSIZE, IMGSIZE);

                if ( sendDiffImage(img1, img2, iSize, imgStruct) )
                    imgStrctCount += 1;
            } else if (x < IMGSIZE && y >= IMGSIZE) {
                const QImage &img1 = _snap1.copy(iSize.width(), iSize.height(), x, IMGSIZE);
                const QImage &img2 = _snap2.copy(iSize.width(), iSize.height(), x, IMGSIZE);

                if ( sendDiffImage(img1, img2, iSize, imgStruct) )
                    imgStrctCount += 1;
            } else if (x >= IMGSIZE && y < IMGSIZE) {
                const QImage &img1 = _snap1.copy(iSize.width(), iSize.height(), IMGSIZE, y);
                const QImage &img2 = _snap2.copy(iSize.width(), iSize.height(), IMGSIZE, y);

                if ( sendDiffImage(img1, img2, iSize, imgStruct) )
                    imgStrctCount += 1;
            } else {
                const QImage &img1 = _snap1.copy(iSize.width(), iSize.height(), x, y);
                const QImage &img2 = _snap2.copy(iSize.width(), iSize.height(), x, y);

                if ( sendDiffImage(img1, img2, iSize, imgStruct) )
                    imgStrctCount += 1;
            }

            iSize += QSize(0, IMGSIZE);
        }
        iSize.setHeight(0);
        iSize += QSize(IMGSIZE, 0);
    }

    _snap1.swap(_snap2);

    QByteArray arr;
    arr.append(GMOD_IMG);
    arr.append(toBytes((quint16)imgStrctCount));
    arr.append(imgStruct);
    sendData(arr);

    qDebug() << Q_FUNC_INFO << imgStrctCount << arr.size();
}


bool ModGraphics::sendDiffImage(const QImage &pix1, const QImage &pix2, const QSize &size, QByteArray &arr)
{
    if (pix1 == pix2)
        return false;

    QByteArray byteImage;
    //byteImage.reserve(1024 * 10);
    QBuffer buffer(&byteImage);
    buffer.open(QIODevice::WriteOnly);
    pix1.save(&buffer, _format.toLocal8Bit());

    QByteArray array;
    array.append(toBytes((quint16)size.width()));
    array.append(toBytes((quint16)size.height()));
    array.append(toBytes((quint16)byteImage.size()));
    array.append(byteImage);

    arr.append(array);

//    int x, y, s;
//    x = fromBytes<quint16>(array.mid(0, 2));
//    y = fromBytes<quint16>(array.mid(2, 2));
//    s = fromBytes<quint16>(array.mid(4, 2));

//    qDebug() << Q_FUNC_INFO << s << x << y;

    return true;
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
        XTestFakeButtonEvent((Display*)dspl, 1, 1, 100);
    }
    if (click & GMCLICK_MIDDLE) {
        XTestFakeButtonEvent((Display*)dspl, 2, 1, 100);
    }
    if (click & GMCLICK_RIGHT) {
        XTestFakeButtonEvent((Display*)dspl, 3, 1, 100);
    }
#endif
}


void ModGraphics::clickUpFunc(GMCLICK click, void *dspl)
{
#ifdef Q_WS_X11
    if (click & GMCLICK_LEFT) {
        XTestFakeButtonEvent((Display*)dspl, 1, 0, 100);
    }
    if (click & GMCLICK_MIDDLE) {
        XTestFakeButtonEvent((Display*)dspl, 2, 0, 100);
    }
    if (click & GMCLICK_RIGHT) {
        XTestFakeButtonEvent((Display*)dspl, 3, 0, 100);
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
    sendData(arr);
//    client_->sendLevelTwo(mode_, adminId_, arr);
}

}


