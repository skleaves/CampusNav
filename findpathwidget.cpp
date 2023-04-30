#include "findpathwidget.h"
#include "ui_findpathwidget.h"

FindPathWidget::FindPathWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindPathWidget)
{
    ui->setupUi(this);

    ui->comboBoxStart->setView(new QListView());
    ui->comboBoxStart->setEditable(true);
    ui->comboBoxStart->setMaxVisibleItems(100);
    ui->comboBoxEnd->setView(new QListView());
    ui->comboBoxEnd->setEditable(true);
    ui->comboBoxEnd->setMaxVisibleItems(100);

}

FindPathWidget::~FindPathWidget()
{
    delete ui;
}

void FindPathWidget::loadItems(QStringList str)
{
    ui->comboBoxStart->addItems(str);
    ui->comboBoxEnd->addItems(str);
    //模糊搜素
    QCompleter *pCompleter = new QCompleter(str, this);
    pCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    pCompleter->setFilterMode(Qt::MatchContains);
    ui->comboBoxStart->setCompleter(pCompleter);
    ui->comboBoxEnd->setCompleter(pCompleter);
    ui->comboBoxStart->setCurrentIndex(-1);
    ui->comboBoxEnd->setCurrentIndex(-1);
}
