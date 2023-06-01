#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->toolBar->setIconSize(QSize(40,40));
    ui->toolBar->setStyleSheet("QToolBar {border-right: 2px;}");
    ui->toolBar->layout()->setContentsMargins(5, 5, 5, 5);	//设置周围间隔
    ui->toolBar->layout()->setSpacing(10);	//设置部件之间的间隔
    QFont font("Microsoft YaHei");
    font.setPointSize(10);

    findPathWidget = new FindPathWidget();
    ui->stackedWidget->addWidget(findPathWidget);
    posWidget = new PosWidget();
    ui->stackedWidget->addWidget(posWidget);
    pathWidget = new PathWidget();
    ui->stackedWidget->addWidget(pathWidget);
    ui->stackedWidget->setCurrentWidget(findPathWidget);



    QActionGroup* normalAddActions = new QActionGroup(this);
    normalAddActions->setExclusive(true);

    normal_action = ui->toolBar->addAction("查看地图");
    normalAddActions->addAction(normal_action);
    normal_action->setParent(ui->toolBar);
    normal_action->setCheckable(true);
    normal_action->setChecked(true);
    normal_action->setFont(font);
    normal_action->setIcon(QIcon(":/img/normal.png"));

    add_loc_action = ui->toolBar->addAction("编辑地点");
    normalAddActions->addAction(add_loc_action);
    add_loc_action->setParent(ui->toolBar);
    add_loc_action->setCheckable(true);
    add_loc_action->setFont(font);
    add_loc_action->setIcon(QIcon(":/img/pos.png"));

    add_path_action = ui->toolBar->addAction("添加路径");
    normalAddActions->addAction(add_path_action);
    add_path_action->setParent(ui->toolBar);
    add_path_action->setCheckable(true);
    add_path_action->setFont(font);
    add_path_action->setIcon(QIcon(":/img/path.png"));
    ui->toolBar->addSeparator();

    QAction *save_action = ui->toolBar->addAction("保存地图信息");
    save_action->setParent(ui->toolBar);
    save_action->setFont(font);
    save_action->setIcon(QIcon(":/img/baocun.png"));
    QAction *load_action = ui->toolBar->addAction("加载地图信息");
    load_action->setParent(ui->toolBar);
    load_action->setFont(font);
    load_action->setIcon(QIcon(":/img/shuaxin.png"));
    QAction *clear_action = ui->toolBar->addAction("清除地图信息");
    clear_action->setParent(ui->toolBar);
    clear_action->setFont(font);
    clear_action->setIcon(QIcon(":/img/clear.png"));



    QLabel *viewCoord = new QLabel("view: 0, 0",this);
    viewCoord->setMinimumWidth(150);
    ui->statusBar->addWidget(viewCoord);
    QLabel *sceneCoord = new QLabel("scene: 0, 0",this);
    sceneCoord->setMinimumWidth(150);
    ui->statusBar->addWidget(sceneCoord);
    QLabel *mapCoord = new QLabel("item: 0, 0",this);
    mapCoord->setMinimumWidth(150);
    ui->statusBar->addWidget(mapCoord);

    currentPos = new QLabel("",this);
    currentPos->setMinimumWidth(150);
    ui->statusBar->addPermanentWidget(currentPos);

    //显示图片
    //QImage img1("C:\\Users\\atsky\\Desktop\\test.png");
    QImage img1(":/img/map.png");
    //ui->graphicsView->setStyleSheet("{border: 2px solid gray;}");
    ui->graphicsView->sceneCoord = sceneCoord;
    ui->graphicsView->viewCoord = viewCoord;
    ui->graphicsView->mapCoord = mapCoord;
    ui->graphicsView->currentPos = currentPos;
    ui->graphicsView->setImage(img1);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->graphicsView->setMouseTracking(true);

    tableWidget = new TableWidget();
    tableWidget->m_map = ui->graphicsView->m_map;
    tableWidget->loadTableView();

    //绑定各个控件对应的信号与槽函数
    connect(normal_action, &QAction::triggered, this, &MainWindow::onActionNormal);
    connect(add_loc_action, &QAction::triggered, this, &MainWindow::onActionPos);
    connect(add_path_action, &QAction::triggered, this, &MainWindow::onActionPath);

    connect(normal_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionNormal);
    connect(add_loc_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionModPos);
    connect(add_path_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionAddPath);
    connect(save_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionSave);
    connect(load_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionLoad);
    connect(clear_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionClear);

    connect(this->ui->graphicsView, &MyGraphicsView::stateChanged, this, &MainWindow::onStateChanged);
    connect(this->ui->graphicsView, &MyGraphicsView::getUserInput, this, &MainWindow::onGetUserInput);
    connect(this->ui->graphicsView, &MyGraphicsView::posChanged, this, &MainWindow::onPosChanged);
    connect(this->ui->graphicsView, &MyGraphicsView::showSelectedPos, this, &MainWindow::onShowSelectedPos);

    connect(this->findPathWidget, &FindPathWidget::pushBtnFindPressed, this, &MainWindow::onPushBtnFindPressed);
    connect(this->findPathWidget, &FindPathWidget::pushBtnClearPressed, this, &MainWindow::onPushBtnClearPressed);

    connect(this->posWidget, &PosWidget::btnAddToggled, this->ui->graphicsView, &MyGraphicsView::onActionAddPos);
    connect(this->posWidget, &PosWidget::showTableWidget, this, &MainWindow::onShowTableWidget);
    connect(this->posWidget, &PosWidget::lineNameEdited, this, &MainWindow::onlineNameEdited);
    connect(this->posWidget, &PosWidget::delSelectedPos, this, &MainWindow::onDelSelectedPos);

    connect(this->tableWidget, &TableWidget::posNameEdited, this, &MainWindow::onPosNameEdited);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onActionNormal(bool checked)
{
    if (checked) {
        this->ui->stackedWidget->setCurrentWidget(findPathWidget);
        posWidget->setBtnAdd(false);
    }
}

void MainWindow::onActionPos(bool checked)
{
    if (checked) {
        this->ui->stackedWidget->setCurrentWidget(posWidget);
        posWidget->setBtnAdd(false);
    }
}

void MainWindow::onActionPath(bool checked)
{
    if (checked) {
        this->ui->stackedWidget->setCurrentWidget(pathWidget);
        posWidget->setBtnAdd(false);
    }
}



void MainWindow::onPosChanged()
{
    //先清空现有的map
    findPathWidget->nameToId.clear();

    ui->graphicsView->m_map->dist_cache.clear();
    ui->graphicsView->m_map->path_cache.clear();

    QStringList strList;

    //需要更新下拉列表
    for (auto p : ui->graphicsView->m_map->m_all_locs) {
        QString str;
        if (p->isBuild) {
            str.append(p->name);
            for (auto it = p->otherName.begin(); it != p->otherName.end(); it ++) {
                str.append(" | ");
                str.append(*it);
            }
            strList.append(str);
            findPathWidget->nameToId.insert(str, p->id);
        }
    }
    strList.sort();

    findPathWidget->loadItems(strList);
    qDebug() << "reload table";
    tableWidget->loadTableView();
}

void MainWindow::onStateChanged(int state)
{
    switch (state) {
    case 0:     //普通模式
        this->ui->graphicsView->setCursor(Qt::ArrowCursor);
        normal_action->setChecked(true);
        break;
    case 1:     //添加地点
        this->ui->graphicsView->setCursor(Qt::CrossCursor);
        add_loc_action->setChecked(true);
        break;
    case 2:     //预添加路径
        this->ui->graphicsView->setCursor(Qt::PointingHandCursor);
        add_path_action->setChecked(true);
        break;
    case 3:     //添加路径
        this->ui->graphicsView->setCursor(Qt::PointingHandCursor);
        add_path_action->setChecked(true);
        break;
    case 4:     //编辑地点
        this->ui->graphicsView->setCursor(Qt::ArrowCursor);
        add_loc_action->setChecked(true);
        posWidget->setBtnAdd(false);
        break;
    }
}

void MainWindow::onGetUserInput(bool &isOK, QString &str)
{
    bool OK = false;
    str = QInputDialog::getText(this, "添加地点", "请输入地点名",
                               QLineEdit::Normal, "", &OK);
    isOK = OK;
}

void MainWindow::onShowSelectedPos(Pos * pos)
{
    if (pos == NULL || !pos->isBuild) {
        if (pos == NULL) currentPos->clear();
        //else currentPos->setText(QString(pos->id));
        else qDebug() << pos->id;
        posWidget->showPosName(pos);
        posWidget->setEditEnable(false);
        return;
    }

    posWidget->setEditEnable(true);
    posWidget->showPosName(pos);

    QString str;
    str.append(pos->name);
    if (pos->otherName.size() > 0) {
        for (auto s : pos->otherName) {
            str.append(" | ");
            str.append(s);
        }
    }
    currentPos->setText(str);
}

void MainWindow::onlineNameEdited(QString name, QStringList otherName)
{
    if (ui->graphicsView->selectedItem != NULL) {
        Pos *pos = ui->graphicsView->selectedItem->getPosition();
        pos->name = name;
        pos->otherName = otherName;

        QString str;
        str.append(pos->name);
        if (pos->otherName.size() > 0) {
            for (auto s : pos->otherName) {
                str.append(" | ");
                str.append(s);
            }
        }
        currentPos->setText(str);

        //需要更新到下拉列表
        onPosChanged();
    }
}

void MainWindow::onPosNameEdited(int id, QString name, QStringList otherName)
{
    Pos * pos = ui->graphicsView->m_map->m_all_locs[ui->graphicsView->m_map->idToIdx[id]];

    pos->name = name;
    pos->otherName = otherName;

    if (ui->graphicsView->selectedItem != NULL) {
        Pos *selPos = ui->graphicsView->selectedItem->getPosition();
        if (selPos->id == pos->id) {
            QString str;
            str.append(pos->name);
            if (pos->otherName.size() > 0) {
                for (auto s : pos->otherName) {
                    str.append(" | ");
                    str.append(s);
                }
            }
            currentPos->setText(str);

            posWidget->showPosName(selPos);
        }
    }

    //需要更新到下拉列表
    onPosChanged();
}

void MainWindow::onShowTableWidget()
{
    qDebug() << "show table";


    tableWidget->show();
}

void MainWindow::onDelSelectedPos()
{
    if (ui->graphicsView->selectedItem != NULL) {
        Pos *pos = ui->graphicsView->selectedItem->getPosition();

        //在邻接表中删除该点
        //首先需要找到和这个点邻接的点
        QLinkedList<QPairI> adjList;    //这个点的邻接表
        for (auto t : ui->graphicsView->m_map->m_adjList) {
            if (t.first().first == pos->id) {
                adjList = t;
                ui->graphicsView->m_map->m_adjList.removeOne(t);
                break;
            }
        }
        QList<int> adjPos;  //与该点邻接的所有点的id
        for (auto it = adjList.begin() + 1; it != adjList.end(); it ++) {
            adjPos.push_back((*it).first);
        }
        //然后对所有邻接的点 都需要在其邻接表中删除该点
        for (auto pid : adjPos) {
            for (auto it = ui->graphicsView->m_map->m_adjList.begin(); it != ui->graphicsView->m_map->m_adjList.end(); it ++) {
                if ((*it).first().first == pid) {
                    for (auto pair : (*it)) {
                        if (pair.first == pos->id) {
                            it->removeOne(pair);
                            break;
                        }
                    }
                    break;
                }
            }
        }

        //在所有loc中删除
        ui->graphicsView->m_map->m_all_locs.removeOne(pos);

        ui->graphicsView->m_map->idToIdx.clear();
        for (int i = 0; i < ui->graphicsView->m_map->m_all_locs.size(); i ++) {
            ui->graphicsView->m_map->idToIdx.insert(ui->graphicsView->m_map->m_all_locs[i]->id, i);
        }

        //在所有edge中删除
        for (auto it = ui->graphicsView->m_map->m_all_edges.begin(); it != ui->graphicsView->m_map->m_all_edges.end();) {
            if ((*it)->start_pos == pos->id || (*it)->end_pos == pos->id) ui->graphicsView->m_map->m_all_edges.erase(it);
            else it ++;
        }

        ui->graphicsView->m_all_locs_list.removeOne(ui->graphicsView->selectedItem);


        //删除边图元
        auto items = ui->graphicsView->selectedItem->collidingItems();
        for (auto item : items) {
            if (item->type() == 6) {
                ui->graphicsView->m_all_edges_list.removeOne(static_cast<QGraphicsLineItem*>(item));
                delete item;
            }
        }

        delete ui->graphicsView->selectedItem;

        //需要更新到下拉列表
        onPosChanged();
        return;
    }
}

void MainWindow::onPushBtnFindPressed(int start, int end)
{
    ui->graphicsView->clearPathLine();
    qDebug() << "计算最短路" << start << end;
    int len = ui->graphicsView->m_map->m_adjList.size();
    if (!ui->graphicsView->m_map->dist_cache.contains(start)) {
        ui->graphicsView->m_map->dijkstra(start, len);
        qDebug() << "计算成功";
    }
    ui->graphicsView->showPath(start, end);
}

void MainWindow::onPushBtnClearPressed()
{
    ui->graphicsView->clearPathLine();
}
