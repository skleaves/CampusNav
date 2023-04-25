#ifndef MYGRAPHICSITEM_H
#define MYGRAPHICSITEM_H

#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>
#include <QPropertyAnimation>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
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

     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE
     {
         QStyleOptionGraphicsItem op;
         op.initFrom(widget);
         if (option->state & QStyle::State_Selected) {
            op.state = QStyle::State_None;
         }
         QGraphicsEllipseItem::paint(painter, &op, widget);
     }

     void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
     void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
     void hoverEnterEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
     //void hoverMoveEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
     void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
};

#endif // MYGRAPHICSITEM_H
