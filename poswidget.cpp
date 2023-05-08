#include "poswidget.h"
#include "ui_poswidget.h"

PosWidget::PosWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PosWidget)
{
    ui->setupUi(this);
    ui->pushBtnAdd->setIcon(QIcon(":/img/addpos.png"));
    ui->pushBtnAdd->setCheckable(true);
    ui->pushBtnAdd->setFocusPolicy(Qt::NoFocus);
    ui->pushBtnDel->setFocusPolicy(Qt::NoFocus);

    connect(ui->pushBtnAdd, &QPushButton::toggled, this, &PosWidget::onBtnAdd);
    connect(ui->line1st, &QLineEdit::editingFinished, this, &PosWidget::onLineEditingFinished);
    connect(ui->line2nd, &QLineEdit::editingFinished, this, &PosWidget::onLineEditingFinished);
}

PosWidget::~PosWidget()
{
    delete ui;
}

void PosWidget::showPosName(QVector<QString> name)
{
    if (name.size() == 0) {
        ui->line1st->setText("");
        ui->line2nd->setText("");
        return;
    }
    QString str;
    str = name.takeFirst();
    ui->line1st->setText(str);
    str.clear();
    while (name.size() > 0) {
        str.append(name.takeFirst());
        str.append(" ");
    }
    ui->line2nd->setText(str);
}

void PosWidget::setBtnAdd(bool checked)
{
    if (checked) ui->pushBtnAdd->setChecked(true);
    else ui->pushBtnAdd->setChecked(false);
}

void PosWidget::onBtnAdd(bool checked)
{
    emit btnAddToggled(checked);
}

void PosWidget::onLineEditingFinished()
{
    QVector<QString> name;
    name.push_back(ui->line1st->text());
    if (ui->line2nd->text() != "") name.push_back(ui->line2nd->text());
    emit lineNameEdited(name);
}
