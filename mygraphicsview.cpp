#include "mygraphicsview.h"
#include <QtDebug>

MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    connect(this, &MyGraphicsView::read_MapData, this, &MyGraphicsView::on_readMapData);
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    // 获取当前鼠标相对于view的位置
    QPointF cursorPoint = event->pos();
    // 获取当前鼠标相对于scene的位置
    QPointF scenePos = this->mapToScene(QPoint(cursorPoint.x(), cursorPoint.y()));
    // 获取view的宽高
    qreal viewWidth = this->viewport()->width();
    qreal viewHeight = this->viewport()->height();
    // 获取当前鼠标位置相当于view大小的横纵比
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

        // 将scene坐标转换为放大缩小后的坐标
        QPointF viewPoint = this->matrix().map(scenePos);
        // 通过滚动条控制view放大缩小后的展示scene的位置
        horizontalScrollBar()->setValue(int(viewPoint.x() - viewWidth * hScale));
        verticalScrollBar()->setValue(int(viewPoint.y() - viewHeight * vScale));
    }
    QGraphicsView::wheelEvent(event);
}


void MyGraphicsView::setImage(QImage img)
{
    //把影像添加到画布
    QPixmap Images = QPixmap::fromImage(img);
    QGraphicsPixmapItem *map = new QGraphicsPixmapItem(Images);

    map->setFlag(QGraphicsPixmapItem::ItemIsSelectable, true);
    map->setFlag(QGraphicsPixmapItem::ItemIsMovable, false);
    map->setFlag(QGraphicsPixmapItem::ItemSendsGeometryChanges,true);
    QGraphicsScene *scene = new QGraphicsScene();
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
}

void MyGraphicsView::setScale(qreal scale)
{
    this->scale(scale, scale);
    m_scalnum *= scale;
    qDebug() << m_scalnum;
}

Pos * MyGraphicsView::addPoint(QPointF p)
{
    //需要弹出输入窗口获取地点名
    //if (!isOK) return NULL;
    Pos *pos = new Pos(p.x(), p.y(), true);
    m_all_locs.push_back(pos);
    qDebug() << pos->id;
    drawPoint(pos);

    bool isOK;
    QString name;
    emit getUserInput(isOK, name);
    pos->name = name;

    return pos;
}

Pos * MyGraphicsView::addPathPoint(QPointF p)
{
    Pos *pos = new Pos(p.x(), p.y(), false);
    m_all_locs.push_back(pos);
    qDebug() << pos->id;
    drawPathPoint(pos);
    return pos;
}

void MyGraphicsView::addLine(int pid1, int pid2)
{
    double len = Edge::dist(m_all_locs[pid1]->x, m_all_locs[pid1]->y,
                            m_all_locs[pid2]->x, m_all_locs[pid2]->y);
    Edge *edge = new Edge(pid1, pid2, len);
    m_all_edges.push_back(edge);
    drawLine(edge);
}

void MyGraphicsView::drawPoint(Pos *p)
{
    MyGraphicsItem *pitem = new MyGraphicsItem(0, 0, 40, 40);
    pitem->setZValue(3);
    pitem->setPosition(p);
    pitem->setOpacity(0.5);
    pitem->setPos(p->x - 20, p->y - 20);
    pitem->setPen(QPen(Qt::NoPen));
    pitem->setBrush(QBrush(Qt::blue));
    pitem->setAcceptHoverEvents(true);
    pitem->setAcceptedMouseButtons(Qt::LeftButton);
    pitem->setFlag(MyGraphicsItem::ItemIsSelectable, true);
    pitem->setFlag(MyGraphicsItem::ItemIsMovable, true);
    m_all_locs_list.append(pitem);
    this->scene()->addItem(pitem);
}

void MyGraphicsView::drawPathPoint(Pos *p)
{
    MyGraphicsItem *pitem = new MyGraphicsItem(0, 0, 30, 30);
    m_all_locs_list.append(pitem);
    pitem->setZValue(3);
    pitem->setPosition(p);
    pitem->setOpacity(0.5);
    pitem->setPos(p->x - 15, p->y - 15);
    pitem->setPen(QPen(Qt::NoPen));
    pitem->setBrush(QBrush(Qt::yellow));
    this->scene()->addItem(pitem);
}

void MyGraphicsView::drawLine(Edge *e)
{
    int x1 = m_all_locs[e->start_pos]->x, x2 = m_all_locs[e->end_pos]->x;
    int y1 = m_all_locs[e->start_pos]->y, y2 = m_all_locs[e->end_pos]->y;
    QGraphicsLineItem *line = new QGraphicsLineItem(x1, y1, x2, y2);
    m_all_edges_list.append(line);
    line->setZValue(1);
    QPen pen(Qt::blue);
    pen.setWidth(10);
    pen.setStyle(Qt::DashDotLine);
    line->setPen(pen);
    this->scene()->addItem(line);
}

void MyGraphicsView::clearPoint()
{
    Pos::cnt = 0;
    m_all_locs.clear();
    while (!m_all_locs_list.isEmpty())
        delete m_all_locs_list.takeFirst();
}

void MyGraphicsView::clearLine()
{
    m_all_edges.clear();
    while (!m_all_edges_list.isEmpty())
        delete m_all_edges_list.takeFirst();
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) //准备移动画布
    {
//        if (m_state == M_ADD_PATH) {
//            m_state = M_DEFAULT;
//            return;
//        }
        this->setCursor(Qt::PointingHandCursor);
        m_startpos = mapToScene(event->pos());
    }
    else if (event->button() == Qt::MidButton) {    //恢复初始大小
        QMatrix mat;
        mat.setMatrix(m_scaldft, this->matrix().m12(), this->matrix().m21(), m_scaldft,this->matrix().dx(), this->matrix().dy());
        this->setMatrix(mat);
        m_scalnum = m_scaldft;
    }
    else if (event->button() == Qt::LeftButton){
        QPointF p = this->mapToScene(event->pos());
        //如果在添加目标点模式 则尝试添加新目标点
        if (m_state == M_ADD_LOC) {
            QGraphicsItem *item = NULL;
            item = this->scene()->itemAt(p, this->transform());
            if (item != NULL) {
                //生成透明点 用于检查当前处是否已经有坐标点
                MyGraphicsItem *pitem = new MyGraphicsItem(0, 0, 40, 40);
                pitem->setZValue(3);
                pitem->setOpacity(0);
                pitem->setPos(p.x() - 20, p.y() - 20);
                this->scene()->addItem(pitem);
                if (pitem->collidingItems().size() > 1) {
                    qDebug() << "too close!";
                    delete pitem;
                    return;
                }
            }
            //否则添加并绘制新坐标点
            addPoint(p);
        }

        //若在准备添加路径模式 则尝试添加路径
        else if (m_state == M_ADD_PATHBG) {
            //首先需要点击在一个已有的点作为起点
            QGraphicsItem *item = NULL;
            item = this->scene()->itemAt(p, this->transform());
            if (item != NULL) {
                if (static_cast<MyGraphicsItem*>(item)->type() != MyGraphicsItem::MyItem)
                    emit printLog("请单击起点");
                else {
                    m_state = M_ADD_PATH;
                    m_plast = static_cast<MyGraphicsItem*>(item)->getPosition();
                    emit printLog("起点设置成功");
                }
            }
        }
        //正式添加路径点
        else if (m_state == M_ADD_PATH) {
            MyGraphicsItem *item = static_cast<MyGraphicsItem*>(this->scene()->itemAt(p, this->transform()));
            if (item != NULL) {
                if (item->type() == MyGraphicsItem::MyItem) {
                    addLine(m_plast->id, item->getPosition()->id);
                    emit printLog("路径添加成功");
                    m_plast = item->getPosition();
                    //m_state = M_ADD_PATHBG;
                    return;
                }
                Pos * newp = addPathPoint(p);
                addLine(m_plast->id, newp->id);
                m_plast = newp;
            }
        }
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
    //qDebug() << "[View]: mouse move";
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

void MyGraphicsView::on_readMapData()
{
    qDebug() << m_all_locs.isEmpty();
    qDebug() << m_all_edges.isEmpty();
    for (auto p : m_all_locs) {
        if (p->isBuild == true) drawPoint(p);
        else drawPathPoint(p);
    }
    for (auto e : m_all_edges) {
        drawLine(e);
    }
}

void MyGraphicsView::onActionNormal(bool checked)
{
    if (checked == true) this->m_state = M_DEFAULT;
    this->setCursor(Qt::ArrowCursor);
}

void MyGraphicsView::onActionAddPos(bool checked)
{
    if (checked == true) this->m_state = M_ADD_LOC;
    this->setCursor(Qt::CrossCursor);
    //新建匿名图元 用于预览添加位置
}

void MyGraphicsView::onActionAddPath(bool checked)
{
    if (checked == true) this->m_state = M_ADD_PATHBG;
    this->setCursor(Qt::PointingHandCursor);
}

void MyGraphicsView::onActionSave()
{
    QFile file("map.dat");
    if(!file.open(QIODevice::WriteOnly))
    {
         return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_3);

    out << m_all_locs.size();
    out << m_all_edges.size();
    for (auto p : m_all_locs) {
        out << *p;
    }
    for (auto e : m_all_edges) {
        out << *e;
    }
    qDebug() << file.flush() << m_all_locs.size() << m_all_edges.size();
}

void MyGraphicsView::onActionLoad()
{
    //先清空当前的
    clearPoint();
    clearLine();
    QFile file("map.dat");
    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_3);

    int loc_size, edge_size;
    in >> loc_size >> edge_size;

    for (int i = 0; i < loc_size; i ++) {
        Pos *p = new Pos();
        in >> *p;
        m_all_locs.push_back(p);
    }
    for (int i = 0; i < edge_size; i ++) {
        Edge *e = new Edge();
        in >> *e;
        m_all_edges.push_back(e);
    }
    emit read_MapData();
}

void MyGraphicsView::onActionClear()
{
    clearPoint();
    clearLine();
}
