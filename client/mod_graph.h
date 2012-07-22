#pragma once
#include <QObject>
#include <QPixmap>
#include <QBuffer>
#include <QApplication>
#include <QDesktopWidget>
#include "defines.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace delta3
{
    class mod_graph : public QObject
    {
        Q_OBJECT

    public:
        explicit mod_graph(QObject *parent = 0, quint16 adminId = 0);
        QPixmap snapshot;
        QBuffer buffer;
        QByteArray byteImage;
        void incomeMessage(const QByteArray &data);
        quint16 adminId;
        quint8 quality;
        void sendPix(QByteArray &byteImg);
        void close();
        void mouseClick(GraphMode key, quint16 x, quint16 y);
    public slots:
        void screentick();

    signals:
        void messageReadyRead(ProtocolMode, qint16, QByteArray);
    };
}
