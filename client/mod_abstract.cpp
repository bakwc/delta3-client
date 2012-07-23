#include "mod_abstract.h"
//------------------------------------------------------------------------------
delta3::mod_abstract::mod_abstract(QObject *parent = 0, quint16 adminId = 0)
    : QObject(parent),
      _adminId(adminId)
{

}
//------------------------------------------------------------------------------
