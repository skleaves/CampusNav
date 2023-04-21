#include "mygraphicsview.h"
#include <QtDebug>

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    // 获取当前鼠标相对于view的位置;
    QPointF cursorPoint = event->pos();
    // 获取当前鼠标相对于scene的位置;
    QPointF scenePos = this->mapToScene(QPoint(cursorPoint.x(), cursorPoint.y()));

    // 获取view的宽高;
    qreal viewWidth = this->viewport()->width();
    qreal viewHeight = this->viewport()->height();

    // 获取当前鼠标位置相当于view大小的横纵比例;
    qreal hScale = cursorPoint.x() / viewWidth;
    qreal vScale = cursorPoint.y() / viewHeight;

    if((event->delta() > 0) && (m_scalnum >= 1)) return;
    if((event->delta() < 0) && (m_scalnum <= m_scaldft))//图像缩小到自适应大小之后就不继续缩小
    {
        QMatrix mat;
        mat.setMatrix(m_scaldft, this->matrix().m12(), this->matrix().m21(), m_scaldft,this->matrix().dx(), this->matrix().dy());
        this->setMatrix(mat);
        m_scalnum = m_scaldft;
    }
    else {
        if (event->delta() > 0) setScale(1.1);
        else setScale(0.9);

        // 将scene坐标转换为放大缩小后的坐标;
        QPointF viewPoint = this->matrix().map(scenePos);
        // 通过滚动条控制view放大缩小后的展示scene的位置;
        horizontalScrollBar()->setValue(int(viewPoint.x() - viewWidth * hScale));
        verticalScrollBar()->setValue(int(viewPoint.y() - viewHeight * vScale));
    }

    QGraphicsView::wheelEvent(event);
}


void MyGraphicsView::SetImage(QImage img)
{
    //把影像添加到画布
    QPixmap Images = QPixmap::fromImage(img);
    QGraphicsPixmapItem *map = new QGraphicsPixmapItem(Images);

    map->setFlag(QGraphicsPixmapItem::ItemIsSelectable, true);
    map->setFlag(QGraphicsPixmapItem::ItemIsMovable, false);
    map->setFlag(QGraphicsPixmapItem::ItemSendsGeometryChanges,true);
    QGraphicsScene *scene = new QGraphicsScene(); //场景 = new QGraphicsScene();
    //画布添加至场景
    scene->addItem(map);
    //场景绑定到控件
    this->setScene(scene);
    this->show();

    //设置默认缩放比例
//    int nImgWidth = Images.width();
//    int nImgHeight = Images.height();
//    qreal temp1 = this->width()*1.0/nImgWidth;
//    qreal temp2 = this->height()*1.0/nImgHeight;
//    if(temp1 > temp2) m_scaldft = temp2;
//    else m_scaldft = temp1;
    setScale(m_scaldft);

}

void MyGraphicsView::moveBy(QPointF offset)
{
    //获取滚动条当前位置
    double oposx = this->horizontalScrollBar()->value();
    double oposy = this->verticalScrollBar()->value();

    double nposx = oposx - offset.x()*m_scalnum;
    double nposy = oposy - offset.y()*m_scalnum;

    //设置新的滚轮位置
    this->horizontalScrollBar()->setValue(nposx);
    this->verticalScrollBar()->setValue(nposy);
    this->scene()->update();
    //记录备用
    m_posx = nposx;
    m_posy = nposy;
}

void MyGraphicsView::setScale(qreal scale)
{
    this->scale(scale, scale);
    m_scalnum *= scale;
    qDebug() << m_scalnum;
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        this->setCursor(Qt::PointingHandCursor);
        m_startpos = mapToScene(event->pos());
        //qDebug() << m_startpos;
    }
    else if (event->button() == Qt::MidButton) {    //恢复初始大小
        QMatrix mat;
        mat.setMatrix(m_scaldft, this->matrix().m12(), this->matrix().m21(), m_scaldft,this->matrix().dx(), this->matrix().dy());
        this->setMatrix(mat);
        m_scalnum = m_scaldft;
    }

    QGraphicsView::mousePressEvent(event);
}


void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标弹起进行视图移动
    if (event->button() == Qt::RightButton)
    {
        this->setCursor(Qt::ArrowCursor);
        m_endpos = mapToScene(event->pos());

        //计算鼠标移动的距离
        QPointF offset = m_endpos - m_startpos;
        moveBy(offset);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint viewPoint = event->pos();
    viewCoord->setText(QString::asprintf("view: %d, %d", viewPoint.x(), viewPoint.y()));
    //场景坐标
    QPointF scenePoint = mapToScene(viewPoint);
    sceneCoord->setText(QString::asprintf("scene: %.0f, %.0f", scenePoint.x(), scenePoint.y()));
    //地图坐标
    QGraphicsItem *item = NULL;
    item = this->scene()->itemAt(scenePoint, this->transform());
    if (item != NULL) {
        QPointF itemPoint = item->mapFromScene(scenePoint);
        mapCoord->setText(QString::asprintf("item: %.0f, %.0f", itemPoint.x(), itemPoint.y()));
    }
    QGraphicsView::mouseMoveEvent(event);
}
