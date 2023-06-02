#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include "map.h"
#include "qtablewidget.h"
#include <QMenu>
#include <QAction>
#include <QStandardItemModel>
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

    QTableWidgetItem *m_selectedItem;
    QMenu * m_menu;
    QAction *m_actionDel, *m_actionEmpty;

signals:
    void posNameEdited(int id, QString name, QStringList otherName);

public slots:
    void onPopItemMenu(const QPoint &pos);
    void onPopVerticalHeaderMenu(const QPoint &pos);
    void onPopHorizontalHeaderMenu(const QPoint &pos);

    void onNameEdited(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
};



#endif // TABLEWIDGET_H
