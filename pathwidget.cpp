#include "pathwidget.h"
#include "ui_pathwidget.h"

PathWidget::PathWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathWidget)
{
    ui->setupUi(this);
}

PathWidget::~PathWidget()
{
    delete ui;
}
