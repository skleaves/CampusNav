#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include "map.h"
#include "qtablewidget.h"
#include <QMenu>
#include <QAction>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QLayout>

namespace Ui {
class TableWidget;
}

class TableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableWidget(QWidget *parent = nullptr);
    ~TableWidget();

    void loadTableView();
    void clearTableView();

    Map * m_map;

private:
    Ui::TableWidget *ui;

    QStandardItemModel* m_model;
    QItemSelectionModel* m_selmodel;

    QTableWidgetItem *m_selectedItem;
    QMenu * m_menu;
    QAction *m_actionDel;

signals:
    void posNameEdited(int id, QString name, QStringList otherName);
    void tableSelecteItemChanged(int oid, int nid);
    void tableDelItem(int id);

public slots:
    void onPopItemMenu(const QPoint &pos);
    void onPopVerticalHeaderMenu(const QPoint &pos);
    void onPopHorizontalHeaderMenu(const QPoint &pos);

    void onTableSetSelected(int id);
    void onActionDel();

    void onSlectRow(const QModelIndex &current, const QModelIndex &previous);
    void onNameEdited(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
};



#endif // TABLEWIDGET_H
