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
    ~MyGraphicsItem();
    MyGraphicsItem(qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = Q_NULLPTR);

public:
     enum { MyItem = UserType + 1 };
     int type() const override
     {
        return MyItem;
     }

     void setPosition(Pos *pos);
     Pos * getPosition();

     //重写绘制 去除选中的虚线框 更改为自定义样式
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

     QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

     void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
     void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
     void hoverEnterEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
     //void hoverMoveEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;
     void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) Q_DECL_OVERRIDE;

public slots:
     void onSelectedChanged();
};

#endif // MYGRAPHICSITEM_H
