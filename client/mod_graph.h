#pragma once

#include <QObject>
#include <QPixmap>
#include <QBuffer>
#include <QApplication>
#include <QDesktopWidget>

#include "defines.h"
#include "mod_abstract.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace delta3
{
    class mod_graph : public mod_abstract
    {
        Q_OBJECT

    public:
        explicit mod_graph(QObject *parent = 0, quint16 adminId = 0);
        void incomeMessage(const QByteArray &data);
        void sendPix(QByteArray &byteImg);
        void close();
        void mouseClick(GraphMode key, quint16 x, quint16 y);

    public slots:
        void screentick();

    signals:
        void messageReadyRead(ProtocolMode, qint16, QByteArray);

    private:
        QPixmap     _snapshot;
        QBuffer     _buffer;
        QByteArray  _byteImage;
        quint16     _quality;
    };
}
