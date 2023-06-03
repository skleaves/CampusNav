#include "mygraphicsview.h"
#include <qmath.h>
#include <QtDebug>
#include <iostream>


MyGraphicsView::MyGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setStyleSheet("padding: 0px; border: 0px;");            //无边框
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   //隐藏水平条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);     //隐藏竖条
    z = new Graphicsviewzoom(this);
    z->set_modifiers(Qt::NoModifier);

    m_map = new Map();
    scene = new QGraphicsScene();

    flasher = new FlashObject(NULL);

    connect(this->scene, &QGraphicsScene::selectionChanged, this, &MyGraphicsView::onSelectItem);
    connect(this, &MyGraphicsView::read_MapData, this, &MyGraphicsView::on_readMapData);
    connect(this, &MyGraphicsView::selfStateChanged, this, &MyGraphicsView::onSelfStateChanged);
}

void MyGraphicsView::wheelEvent(QWheelEvent *event)
{
    QGraphicsView::wheelEvent(event);
}


void MyGraphicsView::setImage(QImage img)
{
    //把影像添加到画布
    QPixmap Images = QPixmap::fromImage(img);
    QGraphicsPixmapItem *map = new QGraphicsPixmapItem(Images);

    map->setFlag(QGraphicsPixmapItem::ItemIsSelectable, false);
    map->setFlag(QGraphicsPixmapItem::ItemIsMovable, false);
    map->setFlag(QGraphicsPixmapItem::ItemSendsGeometryChanges,true);

    //画布添加至场景
    scene->addItem(map);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
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
    this->scale(z->m_scaldft, z->m_scaldft);
}

void MyGraphicsView::moveBy(QPointF offset)
{
    //获取滚动条当前位置
    double oposx = this->horizontalScrollBar()->value();
    double oposy = this->verticalScrollBar()->value();

    double nposx = oposx - offset.x()*z->m_scalnum;
    double nposy = oposy - offset.y()*z->m_scalnum;

    //设置新的滚轮位置
    this->horizontalScrollBar()->setValue(nposx);
    this->verticalScrollBar()->setValue(nposy);
    scene->update();
}

Pos * MyGraphicsView::addPoint(QPointF p)
{
    //需要弹出输入窗口获取地点名
    //if (!isOK) return NULL;
    Pos *pos = new Pos(p.x(), p.y(), true);
    m_map->m_all_locs.push_back(pos);
    QLinkedList<QPairI> lk = QLinkedList<QPairI>();
    lk.push_back(QPairI(pos->id, 0));          //初始化新节点链表头
    m_map->m_adjList.push_back(lk);
    m_map->idToIdx.insert(pos->id, m_map->m_all_locs.size() - 1);
    //qDebug() << pos->id;
    drawPoint(pos);

    bool isOK;
    QString name;
    emit getUserInput(isOK, name);
    if (name == "") {
        name = QString("地点%1").arg(pos->id);
    }
    pos->name = name;

    emit posChanged();

    return pos;
}

Pos * MyGraphicsView::addPathPoint(QPointF p)
{
    Pos *pos = new Pos(p.x(), p.y(), false);
    m_map->m_all_locs.push_back(pos);
    QLinkedList<QPairI> lk = QLinkedList<QPairI>();
    lk.push_back(QPairI(pos->id, 0));          //初始化新节点链表头
    m_map->m_adjList.push_back(lk);
    m_map->idToIdx.insert(pos->id, m_map->m_all_locs.size() - 1);
    //qDebug() << pos->id;
    drawPathPoint(pos);
    return pos;
}

void MyGraphicsView::addLine(Pos * a, Pos * b)
{
    for (auto adjs : m_map->m_adjList) {
        if (adjs.first().first == a->id) {
            for (auto p : adjs) {
                if (p.first == b->id) {
                    qDebug() << "已经存在路径";
                    return;
                }
            }
        }
    }

    //todo len在重绘后需要更新
    double len = Edge::dist(a->x, a->y, b->x, b->y);
    Edge *edge = new Edge(a->id, b->id, len);
    m_map->m_all_edges.push_back(edge);
    //在两个节点间建立无向边
    for (auto it = m_map->m_adjList.begin(); it != m_map->m_adjList.end(); it ++) {
        if ((*it).first().first == a->id) {
            it->push_back(QPairI(b->id, len));
        }
        else if ((*it).first().first == b->id) {
            it->push_back(QPairI(a->id, len));
        }
    }
    drawLine(edge);
}

void MyGraphicsView::drawPoint(Pos *p)
{
    MyGraphicsItem *pitem = new MyGraphicsItem(0, 0, 40, 40);
    pitem->setFlag(MyGraphicsItem::ItemIsSelectable, true);
    pitem->setFlag(MyGraphicsItem::ItemSendsGeometryChanges, true);
    pitem->setZValue(3);
    pitem->setPosition(p);
    pitem->setOpacity(0.5);
    pitem->setPos(p->x - 20, p->y - 20);
    pitem->setPen(QPen(Qt::NoPen));
    pitem->setBrush(QBrush(Qt::blue));
    pitem->setAcceptHoverEvents(true);
    m_all_locs_list.append(pitem);
    scene->addItem(pitem);
//    pitem->setFlag(MyGraphicsItem::ItemIsSelectable, true);
//    pitem->setFlag(MyGraphicsItem::ItemIsMovable, true);
//    pitem->setSelected(false);
//    pitem->setAcceptedMouseButtons(Qt::LeftButton);
}

void MyGraphicsView::drawPathPoint(Pos *p)
{
    MyGraphicsItem *pitem = new MyGraphicsItem(0, 0, 30, 30);
    m_all_locs_list.append(pitem);
    pitem->setFlag(MyGraphicsItem::ItemIsSelectable, true);
    pitem->setZValue(3);
    pitem->setPosition(p);
    pitem->setOpacity(0.5);
    pitem->setPos(p->x - 15, p->y - 15);
    pitem->setPen(QPen(Qt::NoPen));
    pitem->setBrush(QBrush(Qt::yellow));
    scene->addItem(pitem);
}

void MyGraphicsView::drawLine(Edge *e)
{
    int st = m_map->idToIdx[e->start_pos], ed = m_map->idToIdx[e->end_pos];
    int x1 = m_map->m_all_locs[st]->x, x2 = m_map->m_all_locs[ed]->x;
    int y1 = m_map->m_all_locs[st]->y, y2 = m_map->m_all_locs[ed]->y;
    //qDebug() << "add path at" << x1 << y1 << "and" << x2 << y2;
    QGraphicsLineItem *line = new QGraphicsLineItem(x1, y1, x2, y2);
    m_all_edges_list.append(line);
    line->setZValue(1);
    QPen pen(Qt::blue);
    pen.setWidth(10);
    pen.setStyle(Qt::DashDotLine);
    pen.setJoinStyle(Qt::RoundJoin);
    line->setPen(pen);
    scene->addItem(line);
}

void MyGraphicsView::drawPathLine(int start, int end)
{
    int st = m_map->idToIdx[start], ed = m_map->idToIdx[end];
    int x1 = m_map->m_all_locs[st]->x, x2 = m_map->m_all_locs[ed]->x;
    int y1 = m_map->m_all_locs[st]->y, y2 = m_map->m_all_locs[ed]->y;
    QGraphicsLineItem *line = new QGraphicsLineItem(x1, y1, x2, y2);
    m_path_list.append(line);
    line->setZValue(2);
    QPen pen(Qt::red);
    pen.setWidth(20);
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    line->setPen(pen);
    scene->addItem(line);
}

void MyGraphicsView::showPath(int start, int end)
{
    QVector<int> p;
    if (m_map->dist_cache.contains(start)) {
        int endidx = m_map->idToIdx[end];
        QVector<QPairI> dist = m_map->dist_cache[start];
        //qDebug() << dist;
        if (fabs(dist[endidx].second - 1e9) < 1e-3) return;
        QVector<QPair<int, int>> path = m_map->path_cache[start];
//        qDebug() << "有start";
//        for (int i = 0; i < path.size(); i ++) {
//            qDebug() << i << path[i];
//        }
//        return;
        p.append(end);

        for (int i = endidx; path[i].second != -1;) {
            p.append(path[i].second);   //上一个点的id
            i = m_map->idToIdx[path[i].second];
        }
        //qDebug() << p;
        //return;
    }
    else return;

    Q_ASSERT(p.size() >= 2);

    for (int i = 0; i < p.size() - 1; i ++) {
        int s = p[i], e = p[i+1];
        drawPathLine(s, e);
    }

}

void MyGraphicsView::clearPoint()
{
    Pos::cnt = 0;
    m_map->m_all_locs.clear();
    while (!m_all_locs_list.isEmpty()) {
        delete m_all_locs_list.takeLast();
    }
}

void MyGraphicsView::clearLine()
{
    m_map->m_all_edges.clear();
    while (!m_all_edges_list.isEmpty()) {
        delete m_all_edges_list.takeLast();
    }
    clearPathLine();
}

void MyGraphicsView::clearPathLine()
{
    while (!m_path_list.isEmpty()) {
        delete m_path_list.takeLast();
    }
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) //准备移动画布
    {
        this->setCursor(Qt::PointingHandCursor);
        isMoving = true;
        m_startpos = mapToScene(event->pos());
    }
    else if (event->button() == Qt::MidButton) {    //恢复初始大小
        QMatrix mat;
        mat.setMatrix(z->m_scaldft, this->matrix().m12(), this->matrix().m21(), z->m_scaldft,this->matrix().dx(), this->matrix().dy());
        this->setMatrix(mat);
        z->m_scalnum = z->m_scaldft;
    }
    else if (event->button() == Qt::LeftButton){
        QPointF p = this->mapToScene(event->pos());
        p.setX(round(p.x()));
        p.setY(round(p.y()));
        //如果在添加目标点模式 则尝试添加新目标点
        if (m_state == M_ADD_LOC) {
            QGraphicsItem *item = NULL;
            item = scene->itemAt(p, this->transform());
            if (item != NULL) {
                //生成透明点 用于检查当前处是否已经有坐标点
                MyGraphicsItem *pitem = new MyGraphicsItem(0, 0, 40, 40);
                pitem->setZValue(3);
                pitem->setOpacity(0);
                pitem->setPos(p.x() - 20, p.y() - 20);
                scene->addItem(pitem);
                //qDebug() << pitem->collidingItems().size();
                if (pitem->collidingItems().size() > 2) {
                    qDebug() << "too close!" << pitem->collidingItems().size();
                    delete pitem;
                    return;
                }
                //否则添加并绘制新坐标点
                delete pitem;
                addPoint(p);
                //清除历史记录
                this->m_phistory.clear();
            }
        }

        //若在准备添加路径模式 则尝试添加路径
        else if (m_state == M_ADD_PATHBG) {
            //首先需要点击在一个已有的点作为起点
            QGraphicsItem *item = NULL;
            item = scene->itemAt(p, this->transform());
            if (item != NULL) {
                if (static_cast<MyGraphicsItem*>(item)->type() != MyGraphicsItem::MyItem) {
                    qDebug() << "请单击起点";
                    emit printLog(GRAY_TEXT("单击地点或路径点以设置起点<br>"));
                }
                else {
                    m_state = M_ADD_PATH;
                    m_plast = static_cast<MyGraphicsItem*>(item)->getPosition();
                    qDebug() << "起点设置成功" << m_plast->id;
                    QString idStr = QString::number(m_plast->id, 10);
                    emit printLog(GRAY_TEXT("起点设置成功:") " <font color=blue>" + idStr + "</font> " " <font color=gray></font><br> ");
                }
            }
        }
        //正式添加路径点
        else if (m_state == M_ADD_PATH) {
            MyGraphicsItem *item = static_cast<MyGraphicsItem*>(scene->itemAt(p, this->transform()));
            if (item != NULL) {
                if (item->type() == MyGraphicsItem::MyItem) {
                    //qDebug() << "终点设置成功" << m_plast->id << item->getPosition()->id;
                    QString sidStr = QString::number(m_plast->id, 10);
                    QString eidStr = QString::number(item->getPosition()->id, 10);
                    addLine(m_plast, item->getPosition());
                    emit printLog(GRAY_TEXT("路径添加成功:") " <font color=red>" + sidStr + "</font> " GRAY_TEXT("->") " <font color=red>" + eidStr + "</font><br> ");
                    m_plast = item->getPosition();
                    QGraphicsView::mousePressEvent(event);  //必须将事件向下传递
                    return;
                }
                Pos * newp = addPathPoint(p);
                QString sidStr = QString::number(m_plast->id, 10);
                QString eidStr = QString::number(newp->id, 10);
                emit printLog(GRAY_TEXT("路径添加成功:") " <font color=red>" + sidStr + "</font> " GRAY_TEXT("->") " <font color=red>" + eidStr + "</font><br> ");
                addLine(m_plast, newp);
                m_plast = newp;
            }
        }
    }
    QGraphicsView::mousePressEvent(event);
}


void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        if (this->m_state == M_ADD_LOC) this->setCursor(Qt::CrossCursor);
        else if (this->m_state == M_ADD_PATHBG || this->m_state == M_ADD_PATH) ;
        else this->setCursor(Qt::ArrowCursor);
        isMoving = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    viewPoint = event->pos();
    viewCoord->setText(QString::asprintf("view: %d, %d", viewPoint.x(), viewPoint.y()));
    //场景坐标
    scenePoint = mapToScene(viewPoint);
    sceneCoord->setText(QString::asprintf("scene: %.0f, %.0f", scenePoint.x(), scenePoint.y()));
    //地图坐标
    QGraphicsItem *item = NULL;
    item = scene->itemAt(scenePoint, this->transform());
    if (item != NULL) {
        QPointF itemPoint = item->mapFromScene(scenePoint);
        mapCoord->setText(QString::asprintf("item: %.0f, %.0f", itemPoint.x(), itemPoint.y()));
    }

    if (isMoving) {
        m_endpos = scenePoint;
        //计算鼠标移动的距离
        QPointF offset = m_endpos - m_startpos;
        moveBy(offset);
        m_startpos = scenePoint;
    }

    if (this->m_state == M_ADD_LOC) {
        //scene->removeItem(prevItem);
        if (prevItem != NULL) {
            delete prevItem;
            prevItem = NULL;
        }
        prevItem = new MyGraphicsItem(0, 0, 40, 40);
        prevItem->setZValue(3);
        prevItem->setOpacity(0.35);
        prevItem->setPos(scenePoint.x() - 20, scenePoint.y() - 20);
        prevItem->setPen(QPen(Qt::NoPen));
        prevItem->setBrush(QBrush(Qt::blue));
        scene->addItem(prevItem);
    }
    else if (this->m_state == M_ADD_PATH) {
        if (prevLine != NULL) {
            delete prevLine;
            prevLine = NULL;
        }
        prevLine = new QGraphicsLineItem(m_plast->x, m_plast->y, scenePoint.x(), scenePoint.y());
        prevLine->setZValue(1);
        prevLine->setOpacity(0.5);
        QPen pen(Qt::blue);
        pen.setWidth(10);
        pen.setStyle(Qt::DashDotLine);
        pen.setJoinStyle(Qt::RoundJoin);
        prevLine->setPen(pen);
        scene->addItem(prevLine);
    }


    //debug  查看item是否被删除
    //qDebug() << this->items().size()-1;

    QGraphicsView::mouseMoveEvent(event);
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    //撤销
    if (event->matches(QKeySequence::Undo)) {
        if (this->m_state == M_ADD_LOC) {
            //需要保证有点
            if (this->m_all_locs_list.size() > 0) {
                //qDebug() << "有点";
                int pid = m_all_locs_list.back()->getPosition()->id;
                //只有在没有边的情况下才允许撤销
                if (this->m_map->m_adjList[pid].size() == 1) {
                    //将图元对象删除
                    delete this->m_all_locs_list.takeLast();
                    //在map中删除点
                    Pos * p = this->m_map->m_all_locs.back();
                    this->m_map->m_all_locs.pop_back();
                    this->m_map->m_adjList.pop_back();

                    this->m_phistory.push(p);   //放入暂存区
                    Pos::cnt --;    //编号要减1
                }
            }
        }
        else if (this->m_state == M_ADD_PATH) {
            //需要保证有边
            if (this->m_all_edges_list.size() > 0) {
                //取上一条路径结束点的id
                int lastid = m_map->m_all_edges.back()->end_pos;
                //删除路径

                //如果不是地点 则结束点也需删除
                if (!m_map->m_all_locs[lastid]->isBuild) {

                }
            }
        }

        //qDebug() << "按下了撤销";
    }
    else if (event->matches(QKeySequence::Redo)) {
        if (this->m_state == M_ADD_LOC) {
            //有历史记录
            if (this->m_phistory.size() > 0) {
                drawPoint(m_phistory.pop());
            }
            else qDebug() << "没有历史记录";
        }
        //qDebug() << "按下了重做";
    }
    else if (event->matches(QKeySequence::Cancel)) {
        qDebug() << "按下了取消";
        if (m_state == M_ADD_PATH) {
            int olds = this->m_state;
            m_state = M_ADD_PATHBG;
            emit selfStateChanged(olds, M_ADD_PATHBG);
            emit stateChanged(M_ADD_PATHBG);
            emit printLog(BLUE_TEXT("起点取消 请重新选择<br>"));
        }
        else if (m_state == M_ADD_LOC) {
            int olds = this->m_state;
            m_state = M_MOD_LOC;
            emit selfStateChanged(olds, M_MOD_LOC);
            emit stateChanged(M_MOD_LOC);
        }
        else {
            int olds = this->m_state;
            this->m_state = M_DEFAULT;
            emit selfStateChanged(olds, M_DEFAULT);
            emit stateChanged(M_DEFAULT);
        }
    }
    //debug 显示邻接表
    else if (event->matches(QKeySequence::SelectAll)) {
        qDebug() << "显示邻接表";
        for (auto lk : this->m_map->m_adjList) {
            while (!lk.isEmpty()) {
                std::cout << lk.first().first << "," << lk.first().second << "->";
                lk.pop_front();
            }
            std::cout << std::endl;
        }
        qDebug() << "显示idToIdx";
        for (int i = 0; i < m_map->m_all_locs.size(); i ++) {
            std::cout << m_map->m_all_locs[i]->id << "-" << m_map->idToIdx[m_map->m_all_locs[i]->id];
            std::cout << std::endl;
        }

        /*
        if (m_map->dist_cache.contains(0)) {
            std::cout << m_map->dist_cache[0][end] << std::endl;
            QVector<int> path = m_map->path_cache[0];
            QVector<int> p;
            p.append(end);
            for (int i = end; i > 0; ) {
                p.append(path[i]);
                i = path[i];
            }
            while (p.size() > 0) {
                std::cout << p.takeLast() << ">";
            }
//            while (path.size() > 0) {
//                std::cout << path.takeFirst() << std::endl;
//            }
            std::cout << std::endl;
        }
        */
    }

    QGraphicsView::keyPressEvent(event);
}

void MyGraphicsView::on_readMapData()
{
    for (auto p : m_map->m_all_locs) {
        if (p->isBuild == true) drawPoint(p);
        else drawPathPoint(p);
    }
    for (auto e : m_map->m_all_edges) {
        drawLine(e);
    }
    clearPathLine();

    //需要初始化pos的id
    Pos::cnt = m_map->m_all_locs.last()->id + 1;

    emit selfStateChanged(this->m_state, this->m_state);
}

void MyGraphicsView::onActionNormal(bool checked)
{
    int olds = this->m_state;
    if (checked == true) this->m_state = M_DEFAULT;
    emit selfStateChanged(olds, M_DEFAULT);
}

void MyGraphicsView::onActionModPos(bool checked)
{
    int olds = this->m_state;
    if (checked == true) this->m_state = M_MOD_LOC;
    emit selfStateChanged(olds, M_MOD_LOC);
}

void MyGraphicsView::onActionAddPos(bool checked)
{
    int olds = this->m_state;
    if (checked == true) {
        this->m_state = M_ADD_LOC;
        emit selfStateChanged(olds, M_ADD_LOC);
    }
    else if (checked == false) {
        this->m_state = M_MOD_LOC;
        emit selfStateChanged(olds, M_MOD_LOC);
    }
}

void MyGraphicsView::onActionAddPath(bool checked)
{
    int olds = this->m_state;
    if (checked == true) this->m_state = M_ADD_PATHBG;
    emit selfStateChanged(olds, M_ADD_PATHBG);
}

void MyGraphicsView::onActionSave()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("存储地图信息"), "D:",
                                                    tr("地图信息文件 (*.mapdat)"));
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)) {
         return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_3);

    out << m_map->m_all_locs.size();
    out << m_map->m_all_edges.size();
    for (auto p : m_map->m_all_locs) {
        out << *p;
    }
    for (auto e : m_map->m_all_edges) {
        out << *e;
    }
    out << m_map->m_adjList;
    out << m_map->idToIdx;
    //qDebug() << file.flush() << m_map->m_all_locs.size() << m_map->m_all_edges.size();
}

void MyGraphicsView::onActionLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("读取地图信息"), "D:",
                                                    tr("地图信息文件 (*.mapdat)"));
    QFile file(fileName);

    //先清空当前的
    clearPoint();
    clearLine();
    //QFile file("map.dat");
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_3);

    int loc_size, edge_size;
    in >> loc_size >> edge_size;

    for (int i = 0; i < loc_size; i ++) {
        Pos *p = new Pos();
        in >> *p;
        m_map->m_all_locs.push_back(p);
    }
    for (int i = 0; i < edge_size; i ++) {
        Edge *e = new Edge();
        in >> *e;
        m_map->m_all_edges.push_back(e);
    }
    in >> m_map->m_adjList;
    in >> m_map->idToIdx;
    emit read_MapData();
    emit posChanged();
}

void MyGraphicsView::onActionClear()
{
    clearPoint();
    clearLine();
    this->m_map->m_adjList.clear();
    emit posChanged();
}

void MyGraphicsView::onSelfStateChanged(int olds, int news)
{
    //如果新状态是默认
    if (news == M_DEFAULT) {
        for (auto p : m_all_locs_list) {
            p->setFlag(MyGraphicsItem::ItemIsMovable, false);
            if (!p->getPosition()->isBuild) p->hide();
        }
        for (auto e : m_all_edges_list) {
            e->hide();
        }
        this->setCursor(Qt::ArrowCursor);
        if (prevItem != NULL) {
            delete prevItem;
            prevItem = NULL;
        }
        if (prevLine != NULL) {
            delete prevLine;
            prevLine = NULL;
        }
    }
    else if (news == M_MOD_LOC || news == M_ADD_LOC || news == M_ADD_PATHBG || news == M_ADD_PATH) {
        if (olds == M_DEFAULT) {
            if (m_all_locs_list.size() > 0) {
                for (auto p : m_all_locs_list) {
                    if (!p->getPosition()->isBuild) {
                        p->show();
                        p->setFlag(MyGraphicsItem::ItemIsMovable, true);
                        p->setAcceptedMouseButtons(Qt::LeftButton);
                    }
                }
            }
            if (m_all_edges_list.size() > 0) {
                for (auto e : m_all_edges_list) {
                    e->show();
                }
            }
            //清除当前显示出的路径
            clearPathLine();
        }

        if (news == M_MOD_LOC) {
            this->setCursor(Qt::ArrowCursor);
            if (prevItem != NULL) {
                delete prevItem;
                prevItem = NULL;
            }
            if (prevLine != NULL) {
                delete prevLine;
                prevLine = NULL;
            }
        }
        else if (news == M_ADD_LOC) {
            this->setCursor(Qt::CrossCursor);
            if (prevLine != NULL) {
                delete prevLine;
                prevLine = NULL;
            }
        }
        else if (news == M_ADD_PATHBG) {
            this->setCursor(Qt::PointingHandCursor);
            if (prevItem != NULL) {
                delete prevItem;
                prevItem = NULL;
            }
            if (prevLine != NULL) {
                delete prevLine;
                prevLine = NULL;
            }
        }
    }
}

void MyGraphicsView::onSelectItem()
{
    QList<QGraphicsItem *> items = this->scene->selectedItems();
    QVector<QString> name;

    if (items.size() == 0 || items.first()->type() != MyGraphicsItem::MyItem) {

        flasher->setFlash(false);
        selectedItem = NULL;

        emit tableSetSelected(-1);
        emit showSelectedPos(NULL);
        return;
    }
    if (items.first()->type() == MyGraphicsItem::MyItem) {
        MyGraphicsItem *p = static_cast<MyGraphicsItem*>(items.first());
        selectedItem = p;

        //设置选中点的闪烁
        if (selectedItem->getPosition()->isBuild) {
            flasher->setFlash(false);
            flasher->item = selectedItem;
            flasher->setFlash(true);
            emit tableSetSelected(selectedItem->getPosition()->id);
        }
        else {
            flasher->setFlash(false);
            emit tableSetSelected(-1);
        }
        emit showSelectedPos(p->getPosition());
    }
}

void MyGraphicsView::onTableSelectedItemChanged(int oid, int nid)
{
    for (auto &p : m_all_locs_list) {
        if (p->getPosition()->id == nid) {
            p->setSelected(true);
        }
        else if (oid != -1 && p->getPosition()->id == oid) {
            p->setSelected(false);
        }
    }
}
