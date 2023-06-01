#include "findpathwidget.h"
#include "ui_findpathwidget.h"
#include <QDebug>
#include <QListView>

FindPathWidget::FindPathWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindPathWidget)
{
    ui->setupUi(this);

    //QFont font = QFont("方正标雅宋_GBK");
    ui->comboBoxStart->setView(new QListView());
    ui->comboBoxStart->setEditable(true);
    ui->comboBoxStart->setMaxVisibleItems(100);
    //ui->comboBoxStart->setFont(font);
    ui->comboBoxEnd->setView(new QListView());
    ui->comboBoxEnd->setEditable(true);
    ui->comboBoxEnd->setMaxVisibleItems(100);

    connect(this->ui->pushBtnFind, &QPushButton::clicked, this, &FindPathWidget::onPushBtnFindClicked);
    connect(this->ui->pushBtnClear, &QPushButton::clicked, this, &FindPathWidget::onPushBtnClearClicked);
    connect(this->ui->comboBoxStart, &QComboBox::editTextChanged, this, &FindPathWidget::onEditTextChanged);

}

FindPathWidget::~FindPathWidget()
{
    delete ui;
}

void FindPathWidget::loadItems(QStringList str)
{
    this->str = str;
    ui->comboBoxStart->clear();
    ui->comboBoxEnd->clear();
    ui->comboBoxStart->addItems(str);
    ui->comboBoxEnd->addItems(str);
    //模糊搜素
    QCompleter *pCompleterStart = new QCompleter(str, this);
    pCompleterStart->setCaseSensitivity(Qt::CaseInsensitive);
    pCompleterStart->setFilterMode(Qt::MatchContains);
    QListView *listViewStart = qobject_cast<QListView*>(pCompleterStart->popup());
    listViewStart->setFont(QFont("方正标雅宋_GBK", 11));
    QCompleter *pCompleterEnd = new QCompleter(str, this);
    pCompleterEnd->setCaseSensitivity(Qt::CaseInsensitive);
    pCompleterEnd->setFilterMode(Qt::MatchContains);
    QListView *listViewEnd = qobject_cast<QListView*>(pCompleterEnd->popup());
    listViewEnd->setFont(QFont("方正标雅宋_GBK", 11));
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

void FindPathWidget::onEditTextChanged(QString str)
{

}
