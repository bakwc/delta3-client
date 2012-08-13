#pragma once

#include "mod_abstract.h"
#include <QDir>

namespace delta3
{

class ModFile : public ModAbstract
{
    Q_OBJECT

    static const quint8 VERSION = 1;

public:
    explicit ModFile(qint16 adminId, Client *client);

    enum FileMode
    {
        FMOD_INFO       = 1,
        FMOD_CD         = 2,
        FMOD_DIR        = 3,
        FMOD_DOWNINFO   = 4,
        FMOD_DOWNLOAD   = 5,
        FMOD_RENAME     = 6,
        FMOD_DEL        = 7,
        FMOD_COPYTO     = 8,
        FMOD_MOVETO     = 9,
        FMOD_DOWNREQ    = 10,
        FMOD_READY      = 101
    };

    void incomeMessage(const QByteArray &);

protected:
    void timerEvent(QTimerEvent *);
    
signals:
    
public slots:

private:
    QDir dir_;
    
};

}
