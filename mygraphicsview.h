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
#include <QStack>
#include <QTimer>
#include <QFileDialog>
#include "map.h"
#include "mygraphicsitem.h"
#include "graphicsviewzoom.h"
#include "flashobject.h"
#include "pathwidget.h"

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MyGraphicsView(QWidget *parent);
    void setImage(QImage img);
    void moveBy(QPointF offset);
    Pos * addPoint(QPointF p);
    Pos * addPathPoint(QPointF p);
    void addLine(Pos * a, Pos * b);
    void drawPoint(Pos *p);
    void drawPathPoint(Pos *p);
    void drawLine(Edge *e);
    void drawPathLine(int start, int end);

    void showPath(int start, int end);

    void clearPoint();
    void clearLine();
    void clearPathLine();

    MyGraphicsItem *selectedItem = NULL;

private:
    enum {
        M_DEFAULT,    //默认 浏览模式
        M_ADD_LOC,    //添加地点
        M_ADD_PATHBG, //准备添加路径
        M_ADD_PATH,
        M_MOD_LOC     //编辑地点模式
    };
    int m_state = M_DEFAULT;    //状态标记

    bool isMoving = false;
    QGraphicsScene *scene;

    Graphicsviewzoom* z;        //用于缩放
    MyGraphicsItem *prevItem = NULL;   //用于预览
    QGraphicsLineItem *prevLine = NULL;

    QPoint viewPoint;
    QPointF scenePoint;
    QPointF m_startpos;         //鼠标开始点 同下用于移动视图
    QPointF m_endpos;           //鼠标结束点
    Pos *m_plast;

    QStack<Pos *> m_phistory;      //用于撤销时暂存点的历史记录

private:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

public:
    FlashObject *flasher;
    Map * m_map;    //存储的图
    QLinkedList<MyGraphicsItem *> m_all_locs_list;       //所有路径点图元的链表 用于删除
    QLinkedList<QGraphicsLineItem *> m_all_edges_list;   //所有边图元的链表 用于删除
    QLinkedList<QGraphicsLineItem *> m_path_list;        //画出的路径图元的链表 用于删除
    QLabel *sceneCoord, *viewCoord, *mapCoord, *currentPos;

//信号
signals:
    void read_MapData();
    void posChanged();

    void stateChanged(int state);
    void selfStateChanged(int olds, int news);
    void printLog(QString str);
    void getUserInput(bool &isOK, QString &str);
    void showSelectedPos(Pos * pos);

    void tableSetSelected(int id);

public slots:
    void on_readMapData();
    void onActionNormal(bool checked);
    void onActionModPos(bool checked);
    void onActionAddPos(bool checked);
    void onActionAddPath(bool checked);
    void onActionSave();
    void onActionLoad();
    void onActionClear();
    void onSelfStateChanged(int olds, int news);
    void onSelectItem();
    void onTableSelectedItemChanged(int oid, int nid);
};

#endif // MYGRAPHICSVIEW_H
