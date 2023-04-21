#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPen>
#include <QLabel>
#include <vector>
#include <QMatrix>
#include "map.h"

class MyGraphicsView : public QGraphicsView
{
public:
    MyGraphicsView(QWidget *parent);
    void SetImage(QImage img);
    void moveBy(QPointF offset);
    void setScale(qreal scale);

private:
    QGraphicsPixmapItem* m_map;
    QGraphicsEllipseItem* Item;
//    qreal m_scaldft = 1;
    QPointF m_startpos; //鼠标开始点
    QPointF m_endpos; //鼠标结束点
    qreal m_scalnum = 1; //缩放系数
    qreal m_scaldft = 0.170438;
    double m_posx; //视图移动参数x
    double m_posy; //试图移动参数y
    int m_flag; //是否进行选点
    std::vector<QPointF> m_points; //选择的点
    std::vector<std::vector<QPointF>> m_allpoints; //选择的所有点
    double  x1 = 0, x2 = 900, y1 = 0, y2 = 606;
private:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
public:
    Map mapMap;
    QPointF scencePos;
    QPen pen;   // 定义一个画笔，设置画笔颜色和宽度
    Pos Start;
    Pos End;
    Pos nowPos;

    QLabel  *sceneCoord,*viewCoord,*mapCoord;
};

#endif // MYGRAPHICSVIEW_H
