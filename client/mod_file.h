#pragma once
#include "mod_abstract.h"

namespace delta3
{

class ModFile : public ModAbstract
{
    Q_OBJECT
public:
    explicit ModFile(qint16 adminId, Client *client);
    
signals:
    
public slots:
    
};

}
