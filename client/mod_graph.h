#pragma once

#include <QObject>
#include <QImage>
#include <QBuffer>
#include <QApplication>
#include <QDesktopWidget>
#include <QBasicTimer>
#include "defines.h"
#include "mod_abstract.h"
#include "utils.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace delta3
{
	class ModGraphics : public ModAbstract
    {
        Q_OBJECT

        static const quint8 IMGSIZE = 96;

    public:
		explicit ModGraphics(qint16 adminId, Client *client);
        void incomeMessage(const QByteArray &data);

        enum GraphicsMode
        {
            GMOD_INFO       = 1,
            GMOD_IMGFULL    = 2,
            GMOD_IMGDIFF    = 3,
            GMOD_KEYEV      = 4,
            GMOD_MMOV       = 5,
            GMOD_MCLICK     = 6,
            GMOD_IMG        = 7
        };

        enum GMCLICK {
            GMCLICK_LEFT    = 1,
            GMCLICK_RIGHT   = 2,
            GMCLICK_MIDDLE  = 4,
            GMCLICK_DOWN    = 8,
            GMCLICK_UP      = 16,
            GMCLICK_CLICK   = 32,
            GMCLICK_DCLICK  = 64
        };

    public slots:
        void screentick();

    protected:
        void timerEvent(QTimerEvent *);
        void clickDownFunc(GMCLICK click, void* dspl);
        void clickUpFunc(GMCLICK click, void* dspl);

    private:
        QString     _format;
        QImage      _snap1;
        QImage      _snap2;
        quint16     _quality;
        QBasicTimer timer;
        bool        init;

        void sendInform();

        void mouseMove(const QByteArray &data);
        void mouseClick(const QByteArray & data);
        bool sendDiffImage(const QImage& pix1, const QImage& pix2,
                           const QPoint& point, QByteArray &arr);
    };
}
