#ifndef MYGRAPHICSITEM_H
#define MYGRAPHICSITEM_H

#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QPropertyAnimation>
#include "map.h"

class MyGraphicsItem : public QGraphicsEllipseItem
{
private:
    Pos *m_position;

public:
    MyGraphicsItem();
    MyGraphicsItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = Q_NULLPTR);

public:
     enum { MyItem = UserType + 1 };
     int type() const override
     {
        return MyItem;
     }

     void setPosition(Pos *pos);
     Pos * getPosition();

     void hoverEnterEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
     //void hoverMoveEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
     void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
};

#endif // MYGRAPHICSITEM_H
