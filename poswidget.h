#ifndef POSWIDGET_H
#define POSWIDGET_H

#include <QWidget>
#include <QCompleter>
#include <QListView>
#include "map.h"

namespace Ui {
class PosWidget;
}

class PosWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PosWidget(QWidget *parent = nullptr);
    ~PosWidget();

    void showPosName(Pos * pos);
    void setEditEnable(bool flag);
    void setBtnAdd(bool checked);

private:
    Ui::PosWidget *ui;
    QStringList str;

signals:
    void btnAddToggled(bool checked);
    void lineNameEdited(QString name, QStringList otherName);
    void showTableWidget();
    void delSelectedPos();

public slots:
    void onBtnAdd(bool checked);
    void onBtnEdit();
    void onBtnDel();
    void onLineEditingFinished();


};

#endif // POSWIDGET_H
