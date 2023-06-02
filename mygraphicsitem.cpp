#include "mygraphicsitem.h"

MyGraphicsItem::MyGraphicsItem()
{


}

MyGraphicsItem::~MyGraphicsItem()
{
    //qDebug() << "item析构函数";
    this->prepareGeometryChange();
}

MyGraphicsItem::MyGraphicsItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x, y, width, height, parent)
{


}

void MyGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QStyleOptionGraphicsItem op;
    op.initFrom(widget);

    //清空选中时样式
    if (option->state & QStyle::State_Selected) {
       op.state = QStyle::State_None;
    }

    QGraphicsEllipseItem::paint(painter, &op, widget);

    //绘制选中时样式
    if (option->state & QStyle::State_Selected) {
        QPainterPath path = this->shape();
        painter->setOpacity(1);
        QPen pen = QPen(Qt::green);
        pen.setStyle(Qt::DotLine);
        pen.setWidth(3);
        painter->setPen(pen);
        //painter->setBrush(QBrush(Qt::cyan));
        painter->drawPath(path);
    }

}

QVariant MyGraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
        //选中状态发生改变后设置不透明度
        case QGraphicsItem::ItemSelectedHasChanged:
            if (this->isSelected()) this->setOpacity(1);
            else this->setOpacity(0.5);
            break;
        default:
            break;
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}

void MyGraphicsItem::setPosition(Pos *pos)
{
    this->m_position = pos;
}

Pos * MyGraphicsItem::getPosition()
{
    return this->m_position;
}


void MyGraphicsItem::doFlash()
{
    if (this->brush().color() == Qt::blue){
        qDebug() << "设置为红色";
        this->setBrush(QBrush(Qt::red));
        //this->brush().setColor(Qt::red);
        //this->update();
    }
    else {
        qDebug() << "设置为蓝色";
        this->setBrush(QBrush(Qt::blue));
        //this->update();
    }
}

void MyGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "[Item]: move";
    //重建所有相连的边

    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void MyGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mousePressEvent(event);
}

void MyGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //debug 点击后就会修改Pos
    QPointF newp = this->pos();
    Pos *p = this->getPosition();
    if (p->isBuild) {
        p->x = newp.x() + 20;
        p->y = newp.y() + 20;
    }
    else {
        p->x = newp.x() + 15;
        p->y = newp.y() + 15;
    }

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
    if (!this->isSelected()) this->setOpacity(0.5);
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}
