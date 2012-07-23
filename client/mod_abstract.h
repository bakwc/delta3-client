#pragma once

#include <QObject>
#include <QByteArray>
// #include "defines.h"

/*
#ifdef Q_OS_WIN
#   include <windows.h>
#endif
*/

namespace delta3
{

class mod_abstract: public QObject
{
    Q_OBJECT

public:
    explicit mod_abstract(QObject *, quint16);
    virtual void incomeMessage(const QByteArray &) = 0;
    virtual void close() = 0;
//    quint16 getAdminId()                                        const;

protected:
    quint16 _adminId;
};

} // namespace delta3
