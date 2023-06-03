#include "poswidget.h"
#include "ui_poswidget.h"
#include <QListView>
#include <QDebug>
#include <QCompleter>

PosWidget::PosWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PosWidget)
{
    ui->setupUi(this);

    setEditEnable(false);

    ui->pushBtnAdd->setText("");
    ui->pushBtnAdd->setFlat(true);
    ui->pushBtnAdd->setIcon(QIcon(":/img/addpos.png"));
    ui->pushBtnAdd->setIconSize(QSize(50, 50));
    ui->pushBtnAdd->setFixedSize(50, 50);
    ui->pushBtnAdd->setStyleSheet("background-color: none;");
    ui->pushBtnAdd->setCheckable(true);
    ui->pushBtnAdd->setFocusPolicy(Qt::NoFocus);

    ui->pushBtnDel->setText("");
    ui->pushBtnDel->setFlat(true);
    ui->pushBtnDel->setIcon(QIcon(":/img/delpos.png"));
    ui->pushBtnDel->setIconSize(QSize(50, 50));
    ui->pushBtnDel->setFixedSize(50, 50);
    ui->pushBtnDel->setStyleSheet("background-color: none;");
    ui->pushBtnDel->setFocusPolicy(Qt::NoFocus);

//    ui->pushBtnGo->setText("");
//    ui->pushBtnGo->setFlat(true);
//    ui->pushBtnGo->setIcon(QIcon(":/img/goto.png"));
//    ui->pushBtnGo->setIconSize(QSize(50, 50));
//    ui->pushBtnGo->setFixedSize(50, 50);
//    ui->pushBtnGo->setStyleSheet("background-color: none;");
//    ui->pushBtnGo->setFocusPolicy(Qt::NoFocus);

    ui->pushBtnEdit->setText("");
    ui->pushBtnEdit->setFlat(true);
    ui->pushBtnEdit->setIcon(QIcon(":/img/bianji.png"));
    ui->pushBtnEdit->setIconSize(QSize(45, 45));
    ui->pushBtnEdit->setFixedSize(50, 50);
    ui->pushBtnEdit->setStyleSheet("background-color: none;");
    ui->pushBtnEdit->setFocusPolicy(Qt::NoFocus);

//    connect(ui->line1st->lineEdit(), &QLineEdit::editingFinished, [=]() {
//       userEditing = true;
//       onLineEditingFinished();
//    });

    connect(ui->pushBtnAdd, &QPushButton::toggled, this, &PosWidget::onBtnAdd);
    connect(ui->pushBtnDel, &QPushButton::clicked, this, &PosWidget::onBtnDel);
    connect(ui->pushBtnEdit, &QPushButton::clicked, this, &PosWidget::onBtnEdit);
    //connect(ui->line1st, &QLineEdit::editingFinished, this, &PosWidget::onLineEditingFinished);
    connect(ui->line1st, &QLineEdit::editingFinished, this, &PosWidget::onLineEditingFinished);
    connect(ui->line2nd, &QLineEdit::editingFinished, this, &PosWidget::onLineEditingFinished);
}

PosWidget::~PosWidget()
{
    delete ui;
}

void PosWidget::showPosName(Pos * pos)
{
    if (pos == NULL) {
        ui->line1st->clear();
        ui->line2nd->clear();
        return;
    }

    ui->line1st->setText(pos->name);

    if (pos->otherName.size() == 0) {
        ui->line2nd->setText("");
    }
    else {
        QString str;
        for (auto it = pos->otherName.begin(); it != pos->otherName.end(); it ++) {
            str.append(*it);
            if (it != pos->otherName.end() - 1) str.append(",");
        }
        ui->line2nd->setText(str);
    }
}

void PosWidget::setEditEnable(bool flag)
{
    ui->line1st->setEnabled(flag);
    ui->line2nd->setEnabled(flag);
    /*
    if (flag) {
        ui->line1st->setEnabled(true);
        ui->line2nd->setEnabled(true);
    }
    else {
        ui->line1st.set
    }
    */
}

void PosWidget::setBtnAdd(bool checked)
{
    if (checked) ui->pushBtnAdd->setChecked(true);
    else ui->pushBtnAdd->setChecked(false);
}

void PosWidget::setBtnDel(bool flag)
{
    ui->pushBtnDel->setEnabled(flag);
}

void PosWidget::onBtnAdd(bool checked)
{
    emit btnAddToggled(checked);
}

void PosWidget::onBtnEdit()
{
    emit showTableWidget();
}

void PosWidget::onBtnDel()
{
    emit delSelectedPos();
}

void PosWidget::onLineEditingFinished()
{
    qDebug() << "编辑了名字 写入";
    QString name;
    QStringList otherName;

    name = ui->line1st->text();
    //别名需要特殊处理
    QString otherNameStr = ui->line2nd->text();
    if (otherNameStr != "") {
        otherName = otherNameStr.split(",");
    }

    emit lineNameEdited(name, otherName);
}
