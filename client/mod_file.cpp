#include "mod_file.h"
#include "defines.h"

#include <QDir>
#include <QString>
#include <QDebug>

using namespace delta3;


mod_file::mod_file(QObject *parent,qint16 _adminId) :
    QObject(parent)
  , currentDir(new QDir(QDir::rootPath()))
  ,adminId(_adminId)

{
    SendDirectory();
}


mod_file::~mod_file()
{
  delete currentDir;
}


void mod_file::SendDirectory()
{
    QStringList list =currentDir->entryList(QDir::AllEntries,QDir::DirsFirst);

    QByteArray data;
    QString prefix;

    if(currentDir->currentPath()== QDir::rootPath())
        prefix=":root:";
    else
    {
        prefix+=":dir:";
        prefix+=currentDir->dirName();
    }
    data.append(prefix.toUtf8());

    for(QStringList::Iterator i=list.begin();i<list.end();i++)
    {
        data.append(i->toUtf8());
        data.append(QString(":").toUtf8());
    };

   emit messageReadyRead(MOD_FILE, adminId, data);

}

void mod_file::incomeMessage(const QByteArray &data)
{
    QString dataString = QString::fromUtf8(data,data.size());
    QStringList commands = dataString.split(":");

    if (commands.isEmpty())
    {
        qDebug()<<"Error.Wrong data in mod_file::incomeMessage";
        return;
    }

    switch(commands.size())
    {
     case 1:
        ChangeDirectory();
        break;
     case 2:
        if(commands.first()=="mkdir")
            {
                MakeDirectory(commands.last());
                break;
            }
        else
            if(commands.first()=="rmdir")
                {
                    RemoveDirectory(commands.last());
                    break;
                }
            else
                if(commands.first()=="get")
                    {
                       // SendFile(commands.last());
                        break;
                    }
                else  //с приёмом файла пока не уверен.Реализую в последнюю очередь.
                    if(commands.first()=="send")
                        {
                            //GetFile();
                            break;
                        }
                    else
                        if(commands.first()=="abs")
                            {
                                ChangeDirectory(commands.last());
                                break;
                            }
      default:
        qDebug()<<"Some mistake";

     };


}

void mod_file::ChangeDirectory()
{
    currentDir->cdUp();
    SendDirectory();
}


void mod_file::ChangeDirectory(const QString& way)
{
    delete currentDir;
    currentDir=new QDir(way);
    SendDirectory();
}

void mod_file::MakeDirectory(const QString &name)
{
    currentDir->mkdir(name);
    SendDirectory();
}

void mod_file::RemoveDirectory(const QString &name)
{
    currentDir->rmdir(name);
    SendDirectory();
}
