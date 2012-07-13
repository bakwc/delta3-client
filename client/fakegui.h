#ifndef FAKEGUI_H
#define FAKEGUI_H

#include <QMainWindow>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
namespace Ui {
class fakegui;
}
class fakegui : public QMainWindow
{
    Q_OBJECT
public:
    explicit fakegui(QWidget *parent = 0);
    QPixmap image;
private:
     Ui::fakegui *ui;
signals:
    
public slots:
    
};

#endif // FAKEGUI_H
