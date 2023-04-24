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

void MyGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    this->setOpacity(1);
    qDebug() << this->getPosition()->id;
    QGraphicsEllipseItem::hoverEnterEvent(event);
}


void MyGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setOpacity(0.5);
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}
