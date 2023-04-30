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


    //绑定各个控件对应的信号与槽函数
    connect(normal_action, &QAction::triggered, this, &MainWindow::onActionNormal);
    connect(add_loc_action, &QAction::triggered, this, &MainWindow::onActionPos);

    connect(normal_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionNormal);
    connect(add_loc_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionModPos);
    connect(add_path_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionAddPath);
    connect(save_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionSave);
    connect(load_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionLoad);
    connect(clear_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionClear);

    connect(this->ui->graphicsView, &MyGraphicsView::stateChanged, this, &MainWindow::onStateChanged);
    connect(this->ui->graphicsView, &MyGraphicsView::getUserInput, this, &MainWindow::onGetUserInput);
    connect(this->ui->graphicsView, &MyGraphicsView::read_MapData, this, &MainWindow::onReadMap);
    connect(this->ui->graphicsView, &MyGraphicsView::showSelectedPos, this, &MainWindow::onShowSelectedPos);

    connect(this->posWidget, &PosWidget::btnAddToggled, this->ui->graphicsView, &MyGraphicsView::onActionAddPos);
    connect(this->posWidget, &PosWidget::lineNameEdited, this, &MainWindow::onlineNameEdited);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onActionNormal(bool checked)
{
    if (checked) this->ui->stackedWidget->setCurrentWidget(findPathWidget);
}

void MainWindow::onActionPos(bool checked)
{
    if (checked) this->ui->stackedWidget->setCurrentWidget(posWidget);
}

void MainWindow::onActionPath(bool checked)
{

}



void MainWindow::onReadMap()
{
    QStringList strList;

    for (auto p : ui->graphicsView->m_map->m_all_locs) {
        QString str;
        if (p->isBuild) {
            if (p->name.size() > 0) str.append(p->name.first());
            for (auto it = p->name.begin()+1; it != p->name.end(); it ++) {
                str.append(" | ");
                str.append(*it);
            }
            strList.append(str);
        }
    }
    strList.sort();

    if (ui->stackedWidget->currentWidget() == findPathWidget) {
        findPathWidget->loadItems(strList);
    }
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
    }
}

void MainWindow::onGetUserInput(bool &isOK, QString &str)
{
    bool OK = false;
    str = QInputDialog::getText(this, "添加地点", "请输入地点名",
                               QLineEdit::Normal, "", &OK);
    isOK = OK;
}

void MainWindow::onShowSelectedPos(QVector<QString> name)
{
    if (ui->stackedWidget->currentWidget() == posWidget) {
        posWidget->showPosName(name);
    }
}

void MainWindow::onlineNameEdited(QVector<QString> name)
{
    if (ui->graphicsView->selectedItem != NULL) {
        Pos *p = ui->graphicsView->selectedItem->getPosition();
        p->name = name;
        QString str;
        if (name.size() > 0) str.append(name.takeFirst());
        if (name.size() > 0) {
            for (auto s : name) {
                str.append(" | ");
                str.append(s);
            }
        }
        ui->graphicsView->currentPos->setText(str);

        //TODO 需要更新到下拉列表
        onReadMap();
    }
}
