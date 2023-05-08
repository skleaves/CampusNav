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

    connect(this->ui->pushBtnFind, &QPushButton::clicked, this, &FindPathWidget::onPushBtnFindClicked);
    connect(this->ui->pushBtnClear, &QPushButton::clicked, this, &FindPathWidget::onPushBtnClearClicked);


}

FindPathWidget::~FindPathWidget()
{
    delete ui;
}

void FindPathWidget::loadItems(QStringList str)
{
    ui->comboBoxStart->clear();
    ui->comboBoxEnd->clear();
    ui->comboBoxStart->addItems(str);
    ui->comboBoxEnd->addItems(str);
    //模糊搜素
    QCompleter *pCompleterStart = new QCompleter(str, this);
    pCompleterStart->setCaseSensitivity(Qt::CaseInsensitive);
    pCompleterStart->setFilterMode(Qt::MatchContains);
    QCompleter *pCompleterEnd = new QCompleter(str, this);
    pCompleterEnd->setCaseSensitivity(Qt::CaseInsensitive);
    pCompleterEnd->setFilterMode(Qt::MatchContains);
    ui->comboBoxStart->setCompleter(pCompleterStart);
    ui->comboBoxEnd->setCompleter(pCompleterEnd);
    ui->comboBoxStart->setCurrentIndex(-1);
    ui->comboBoxEnd->setCurrentIndex(-1);
}

void FindPathWidget::onPushBtnFindClicked()
{
    if (ui->comboBoxStart->currentIndex() == -1 || ui->comboBoxEnd->currentIndex() == -1) {
        return;
    }
    if (ui->comboBoxStart->currentIndex() == ui->comboBoxEnd->currentIndex()) {
        return;
    }
    int start = nameToId[ui->comboBoxStart->currentText()];
    int end = nameToId[ui->comboBoxEnd->currentText()];
    emit pushBtnFindPressed(start, end);
}

void FindPathWidget::onPushBtnClearClicked()
{
    emit pushBtnClearPressed();
}
