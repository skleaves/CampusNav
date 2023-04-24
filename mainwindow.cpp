#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //显示图片
    //QImage img1("C:\\Users\\atsky\\Pictures\\cube-1.png");
    QImage img1("C:\\Users\\atsky\\Desktop\\mmmrc.png");
    ui->graphicsView->sceneCoord = ui->sceneCoord;
    ui->graphicsView->viewCoord = ui->viewCoord;
    ui->graphicsView->mapCoord = ui->mapCoord;
    ui->graphicsView->setImage(img1);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->graphicsView->setMouseTracking(true);


    //绑定各个控件对应的信号与槽函数
    void (QAbstractButton::*radioBtn_signal)(bool) = &QRadioButton::clicked;
    connect(this->ui->radioBtn_Default, radioBtn_signal, this->ui->graphicsView,
            &MyGraphicsView::on_radioBtn_Default_clicked);
    connect(this->ui->radioBtn_AddLoc, radioBtn_signal, this->ui->graphicsView,
            &MyGraphicsView::on_radioBtn_AddLoc_clicked);
    connect(this->ui->radioBtn_AddPath, radioBtn_signal, this->ui->graphicsView,
            &MyGraphicsView::on_radioBtn_AddPath_clicked);

    void (QAbstractButton::*pushBtn_signal)() = &QPushButton::pressed;
    connect(this->ui->pushBtn_Save, pushBtn_signal, this->ui->graphicsView,
            &MyGraphicsView::on_pushBtn_Save_pressed);
    connect(this->ui->pushBtn_Load, pushBtn_signal, this->ui->graphicsView,
            &MyGraphicsView::on_pushBtn_Load_pressed);
    connect(this->ui->pushBtn_Clear, pushBtn_signal, this->ui->graphicsView,
            &MyGraphicsView::on_pushBtn_Clear_pressed);

    connect(this->ui->graphicsView, &MyGraphicsView::printLog, this, &MainWindow::onPrintLog);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPrintLog(QString str)
{
    this->ui->textLog->append(str);
}
