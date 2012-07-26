#pragma once

#include <QObject>
#include "mod_abstract.h"
#include "defines.h"

namespace delta3
{

class Mod_Proxy : public mod_abstract
{
    Q_OBJECT
public:
    explicit Mod_Proxy(QObject *parent = 0, quint16 adminId = 0);

signals:

public slots:

};

}
