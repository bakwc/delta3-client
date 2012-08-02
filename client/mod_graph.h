#pragma once

#include <QObject>
#include <QPixmap>
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

    public:
		explicit ModGraphics(qint16 adminId, Client *client);
        void incomeMessage(const QByteArray &data);

    public slots:
        void screentick();

    protected:
        void timerEvent(QTimerEvent *);
        void clickDownFunc(GMCLICK click, void* dspl);
        void clickUpFunc(GMCLICK click, void* dspl);

    private:
        QString     _format;
        QPixmap     _snapshot;
        QBuffer     _buffer;
        QByteArray  _byteImage;
        quint16     _quality;
        QBasicTimer timer;
        bool        init;

        void sendInform();

        void mouseMove(const QByteArray &data);
        void mouseClick(const QByteArray & data);

    };
}
