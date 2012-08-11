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
    QByteArray send;

    switch (data[0]) {
    case FMOD_INFO:
        // data[1] // version

        break;
    case FMOD_CD: {
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

        client_->sendLevelTwo(mode_, adminId_, send);

        break; }
    case FMOD_DOWNINFO:

        break;
    case FMOD_DOWNLOAD:

        break;
    case FMOD_RENAME:

        break;
    case FMOD_DEL:

        break;
    case FMOD_COPYTO:

        break;
    case FMOD_MOVETO:

        break;
    case FMOD_READY:

        break;
    }
}


void ModFile::timerEvent(QTimerEvent *ev) {
    QByteArray send;
    send.append((quint8)FMOD_INFO);
    send.append(VERSION);

    client_->sendLevelTwo(mode_, adminId_, send);

    killTimer(ev->timerId());
}


}
