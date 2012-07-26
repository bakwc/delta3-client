#pragma once

#include <QObject>
#include <QPixmap>
#include <QBuffer>
#include <QApplication>
#include <QDesktopWidget>

#include "defines.h"
#include "mod_abstract.h"
#include "utils.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace delta3
{
    class Mod_graph : public Mod_abstract
    {
        Q_OBJECT

    public:
        explicit Mod_graph(QObject *parent = 0, quint16 adminId = 0);
        void incomeMessage(const QByteArray &data);
        void close();

    public slots:
        void screentick();

    signals:
        void messageReadyRead(ProtocolMode, qint16, QByteArray);

    private:
        QString     _format;
        QPixmap     _snapshot;
        QBuffer     _buffer;
        QByteArray  _byteImage;
        quint16     _quality;
        void mouseMove(const QByteArray &data);
        void mouseClick(const QByteArray & data);
        void sendPix(QByteArray &byteImg);
    };
}
