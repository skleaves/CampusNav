#ifndef FLASHOBJECT_H
#define FLASHOBJECT_H

#include <QObject>
#include <QTimer>
#include "mygraphicsitem.h"

class FlashObject : public QObject
{
    Q_OBJECT
public:
    FlashObject(MyGraphicsItem *i);

    MyGraphicsItem* item;
    QTimer *timer;

    void setFlash(bool enable);

signals:

public slots:
    void flash();

};

#endif // FLASHOBJECT_H
