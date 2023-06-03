#include "flashobject.h"


FlashObject::FlashObject(MyGraphicsItem *i)
{
    this->timer = new QTimer();
    this->item = i;
    connect(this->timer, &QTimer::timeout, this, &FlashObject::flash);
}

void FlashObject::setFlash(bool enable)
{
    if(enable) {
        timer->start(500);
    }
    else {
        timer->stop();
        if (item != NULL) item->setBrush(QBrush(Qt::blue));
    }
}

void FlashObject::flash()
{
    if (item != NULL) item->doFlash();
}
