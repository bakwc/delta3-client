#include "mod_file.h"
#include "client.h"
#include <QTimerEvent>

namespace delta3 {


ModFile::ModFile(qint16 adminId, Client *client) :
    ModAbstract(MOD_FILE, adminId, client), dir_(QDir::home())
{
    startTimer(500);
}


void ModFile::incomeMessage(const QByteArray &data)
{
    switch (data[0]) {
    case FMOD_INFO:
        // data[1] // version

        break;
    case FMOD_CD: {
        QByteArray send;

        dir_.setPath(QString::fromUtf8(data.mid(1)));
        const QFileInfoList &fileList = dir_.entryInfoList();

        send.append((quint8)FMOD_DIR);
        send.append(toBytes<quint32>((quint32)fileList.size()));

        for (int id = 0; id < fileList.size(); ++id) {
            QByteArray filename = fileList.at(id).fileName().toUtf8();
            send.append(toBytes<quint16>((quint16)filename.size()));
            send.append(filename);
            send.append(fileList.at(id).isDir() ? '\1' : '\0');
        }

        sendData(send);
//        client_->sendLevelTwo(mode_, adminId_, send);

        break; }
    case FMOD_DOWNINFO:

        break;
    case FMOD_DOWNLOAD:

        break;
    case FMOD_RENAME: {
        int sourceSize = fromBytes<quint16>(data.mid(1, 2));
        QString source = QString::fromUtf8(data.mid(3, sourceSize));
        int destSize   = fromBytes<quint16>(data.mid(sourceSize+3, 2));
        QString dest   = QString::fromUtf8(data.mid(sourceSize+5, destSize));

        qDebug() << dir_.absoluteFilePath(source) << dir_.absoluteFilePath(dest);

        QFile::rename(dir_.absoluteFilePath(source), dir_.absoluteFilePath(dest));

        break; }
    case FMOD_DEL:

        break;
    case FMOD_COPYTO:

        break;
    case FMOD_MOVETO:

        break;

    case FMOD_DOWNREQ:

        break;
    case FMOD_READY:

        break;
    }
}


void ModFile::timerEvent(QTimerEvent *ev) {
    QByteArray send;
    send.append((quint8)FMOD_INFO);
    send.append(VERSION);

    sendData(send);
//    client_->sendLevelTwo(mode_, adminId_, send);

    killTimer(ev->timerId());
}


}
