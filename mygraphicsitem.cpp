#include "mygraphicsitem.h"

MyGraphicsItem::MyGraphicsItem()
{

}

MyGraphicsItem::MyGraphicsItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x, y, width, height, parent)
{
}

void MyGraphicsItem::setPosition(Pos *pos)
{
    this->m_position = pos;
}

Pos * MyGraphicsItem::getPosition()
{
    return this->m_position;
}

void MyGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "[Item]: move";
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void MyGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mousePressEvent(event);
}

void MyGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "[Item]: release";
    QPointF newp = event->scenePos();
    Pos *p = this->getPosition();
    p->x = newp.x();
    p->y = newp.y();
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

void MyGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setOpacity(1);
    Pos *p = this->getPosition();
    qDebug() << p->id << p->name << p->x << p->y;
    QGraphicsEllipseItem::hoverEnterEvent(event);
}


void MyGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setOpacity(0.5);
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}
