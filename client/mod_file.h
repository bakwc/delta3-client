#ifndef MOD_FILE_H
#define MOD_FILE_H

#include <QObject>
#include "defines.h"

using namespace delta3;

//--------форвардные определения----------------
class QDir;
class QString;


//-----конец форвардных определений----------------

class mod_file : public QObject
{
    Q_OBJECT
public:
    explicit mod_file(QObject *parent = 0,qint16 _adminId = 0);
    ~mod_file();

    void ChangeDirectory();
    void ChangeDirectory(const QString& way);
    void MakeDirectory(const QString& name);
    void RemoveDirectory(const QString& name);
    void GetFile();                     //с приёмом и отправкой файлов пока хз.Для начала реализую директории.
    void SendFile(const QString& name);

    void incomeMessage(const QByteArray &data);


private:
    QDir *currentDir;
  qint16  adminId;

    void SendDirectory(); //метод,который будет отзываться на изменение currentDir;
    
signals:
    void messageReadyRead(ProtocolMode, qint16, QByteArray);
    
public slots:



    
};

#endif // MOD_FILE_H
