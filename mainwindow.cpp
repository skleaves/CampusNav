#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->toolBar->setIconSize(QSize(40,40));
    ui->toolBar->layout()->setContentsMargins(5, 5, 5, 5);	//设置周围间隔
    ui->toolBar->layout()->setSpacing(10);	//设置部件之间的间隔
    QFont font("Microsoft YaHei");
    font.setPointSize(10);

    QActionGroup* normalAddActions = new QActionGroup(this);
    normalAddActions->setExclusive(true);
    QAction *normal_action = ui->toolBar->addAction("查看地图");
    normalAddActions->addAction(normal_action);
    normal_action->setParent(ui->toolBar);
    normal_action->setCheckable(true);
    normal_action->setChecked(true);
    normal_action->setFont(font);
    normal_action->setIcon(QIcon(":/img/normal.png"));
    QAction *add_loc_action = ui->toolBar->addAction("添加地点");
    normalAddActions->addAction(add_loc_action);
    add_loc_action->setParent(ui->toolBar);
    add_loc_action->setCheckable(true);
    add_loc_action->setFont(font);
    add_loc_action->setIcon(QIcon(":/img/pos.png"));
    QAction *add_path_action = ui->toolBar->addAction("添加路径");
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



    //显示图片
    QImage img1("C:\\Users\\atsky\\Desktop\\test.png");
    //QImage img1(":/img/map.png");
    ui->graphicsView->sceneCoord = ui->sceneCoord;
    ui->graphicsView->viewCoord = ui->viewCoord;
    ui->graphicsView->mapCoord = ui->mapCoord;
    ui->graphicsView->setImage(img1);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->graphicsView->setMouseTracking(true);


    //绑定各个控件对应的信号与槽函数
    connect(normal_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionNormal);
    connect(add_loc_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionAddPos);
    connect(add_path_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionAddPath);
    connect(save_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionSave);
    connect(load_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionLoad);
    connect(clear_action, &QAction::triggered, this->ui->graphicsView, &MyGraphicsView::onActionClear);

    connect(this->ui->graphicsView, &MyGraphicsView::printLog, this, &MainWindow::onPrintLog);
    connect(this->ui->graphicsView, &MyGraphicsView::getUserInput, this, &MainWindow::onGetUserInput);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPrintLog(QString str)
{
    this->ui->textLog->append(str);
}

void MainWindow::onGetUserInput(bool &isOK, QString &str)
{
    bool OK = false;
    str = QInputDialog::getText(this, "添加地点", "请输入地点名",
                               QLineEdit::Normal, "", &OK);
    isOK = OK;
}
