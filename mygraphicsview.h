#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QScrollBar>
#include <QPen>
#include <QLabel>
#include <vector>
#include <QMatrix>
#include <QLinkedList>
#include "map.h"
#include "mygraphicsitem.h"

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent);
    void setImage(QImage img);
    void moveBy(QPointF offset);
    void setScale(qreal scale);
    Pos * addPoint(QPointF p);
    Pos * addPathPoint(QPointF p);
    void addLine(int pid1, int pid2); //应该传入两个id
    void drawPoint(Pos *p);
    void drawPathPoint(Pos *p);
    void drawLine(Edge *e);

    void clearPoint();
    void clearLine();

private:
    enum {
        M_DEFAULT,    //默认 浏览模式
        M_ADD_LOC,    //添加目的点
        M_ADD_PATHBG, //准备添加路径
        M_ADD_PATH
    };
    int m_state = M_DEFAULT;    //状态标记

    QPointF m_startpos;         //鼠标开始点 同下用于移动视图
    QPointF m_endpos;           //鼠标结束点
    Pos *m_plast;
    qreal m_scalnum = 1;        //缩放系数
    qreal m_scaldft = 0.170438; //默认缩放系数

    QVector<Pos *> m_all_locs;    //记录所有路径点
    QVector<Edge *> m_all_edges;


    QLinkedList<MyGraphicsItem *> m_all_locs_list;       //所有路径点图元的链表 用于删除
    QLinkedList<QGraphicsLineItem *> m_all_edges_list;   //所有边图元的链表 用于删除

private:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
public:
    Map myMap;
    QLabel  *sceneCoord,*viewCoord,*mapCoord;

//信号
signals:
    void read_MapData();
    void printLog(QString str);
    void getUserInput(bool &isOK, QString &str);

public slots:
    void on_readMapData();
    void onActionNormal(bool checked);
    void onActionAddPos(bool checked);
    void onActionAddPath(bool checked);
    void onActionSave();
    void onActionLoad();
    void onActionClear();
};

#endif // MYGRAPHICSVIEW_H
