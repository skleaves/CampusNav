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
    ui->graphicsView->SetImage(img1);
    ui->graphicsView->setMouseTracking(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

