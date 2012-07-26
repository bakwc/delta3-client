#include "mod_abstract.h"
//------------------------------------------------------------------------------
delta3::Mod_abstract::Mod_abstract(QObject *parent = 0, quint16 adminId = 0)
    : QObject(parent),
      _adminId(adminId)
{

}
//------------------------------------------------------------------------------
/*
delta3::mod_abstract::getAdminId()                          const
{
    return _adminId;
}
*/
//------------------------------------------------------------------------------
