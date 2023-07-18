#include "tablewidget.h"
#include "ui_tablewidget.h"

TableWidget::TableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableWidget)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowIcon(QIcon(":/img/liebiao.png"));

    //添加布局
    QHBoxLayout *windowLayout = new QHBoxLayout;
    windowLayout->addWidget(ui->tableView);
    this->setLayout(windowLayout);

    m_model = new QStandardItemModel(this);
    //设置列字段名
    m_model->setColumnCount(3);
    m_model->setHeaderData(0,Qt::Horizontal, "地点名");
    m_model->setHeaderData(1,Qt::Horizontal, "别名");
    m_model->setHeaderData(2,Qt::Horizontal, "ID");
    m_model->setRowCount(0);
    ui->tableView->setModel(m_model);

    //允许点击排序
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setCurrentIndex(m_model->index(-1, -1));
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);//使第二列的内容完全显示
    ui->tableView->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->verticalHeader()->setFixedWidth(40);
    //隔行变色
    ui->tableView->setAlternatingRowColors(true);
    //按行选择
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //打开右键菜单
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tableView->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    //ui->tableView->verticalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    //添加菜单
    m_menu = new QMenu();
    //删除按钮
    m_actionDel = new QAction("删除地点",m_menu);
    connect(m_actionDel, &QAction::triggered, this, &TableWidget::onActionDel);
    //添加删除按钮
    m_menu->addAction(m_actionDel);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &TableWidget::onPopVerticalHeaderMenu);
    connect(ui->tableView->horizontalHeader(), &QTableView::customContextMenuRequested, this, &TableWidget::onPopHorizontalHeaderMenu);
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &TableWidget::onSlectRow);
    connect(this->m_model, &QAbstractItemModel::dataChanged, this, &TableWidget::onNameEdited);
}

TableWidget::~TableWidget()
{
    delete ui;
}

void TableWidget::loadTableView()
{
    qDebug() << "loadview";
    m_model->setRowCount(0);
    QVector<Pos *> locs = m_map->m_all_locs;
    int row = 0;
    //m_model->setRowCount(m_map->m_all_locs.size());

    for (auto location : locs) {
        if (location->isBuild) {
            QList<QStandardItem*> list;
            // 设置地点名项
            list << new QStandardItem(location->name);
            // 设置别名项
            if (location->otherName.size() > 0) {
                QString str;
                for (auto it = location->otherName.begin(); it != location->otherName.end(); it ++) {
                    str.append(*it);
                    if (it != location->otherName.end() - 1) str.append(",");
                }
                list << new QStandardItem(str);
            }
            else list << new QStandardItem("");
            //设置ID项
            QStandardItem * IDItem = new QStandardItem();
            int id = location->id;
            IDItem->setData(QVariant(id), Qt::EditRole);
            IDItem->setSelectable(false);
            IDItem->setEditable(false);
            IDItem->setForeground(QBrush(QColor(Qt::darkGray)));
            list << IDItem;

            for (auto &i : list) {
                i->setTextAlignment(Qt::AlignCenter);
            }
            m_model->appendRow(list);
            row++;
        }
    }
}

void TableWidget::clearTableView()
{
    m_model->clear();

}



void TableWidget::onPopItemMenu(const QPoint &pos)
{
    /*
    m_selectedItem = ui->table->itemAt(pos);
    if(m_selectedItem != nullptr){
        m_menu->exec(QCursor::pos());
    }
    */
}

void TableWidget::onPopVerticalHeaderMenu(const QPoint &pos)
{
    qDebug() << "右键";
    int index = ui->tableView->rowAt(pos.y());
    qDebug() << "选中的行：" << index;
    m_menu->exec(QCursor::pos());
}

void TableWidget::onPopHorizontalHeaderMenu(const QPoint &pos)
{
    QMenu *menu = new QMenu;
    QAction *sort_up = new QAction("升序排列");
    QAction *sort_down = new QAction("降序排列");
    menu->addAction(sort_up);
    menu->addAction(sort_down);
    menu->exec(QCursor::pos());
}

void TableWidget::onTableSetSelected(int id)
{
    qDebug() << "地图上选中" << id << "状态改变 修改列表状态";
    if (id == -1) ui->tableView->setCurrentIndex(m_model->index(-1, -1));
    for (int i = 0; i < m_model->rowCount(); i ++) {
        //if (m_model->item(i, 2)->text().toInt() == id) {
        if (m_model->item(i, 2)->data(Qt::EditRole).toInt() == id) {
            if (ui->tableView->currentIndex().row() == i) return;
            ui->tableView->setCurrentIndex(m_model->index(i, 2));
            break;
        }
    }
}

void TableWidget::onActionDel()
{
    int row = ui->tableView->currentIndex().row();
    if (row != -1) {
        int id = m_model->item(row, 2)->data(Qt::EditRole).toInt();
        emit tableDelItem(id);
        m_model->removeRow(row);
    }
}

void TableWidget::onSlectRow(const QModelIndex &current, const QModelIndex &previous)
{
    int row = current.row();
    qDebug() << "选择的行数：" << row << "原行数" << previous.row();
    int nid = -1;
    if (current.row() != -1) nid = m_model->item(row, 2)->data(Qt::EditRole).toInt();
    int oid = -1;
    if (previous.row() != -1) oid = m_model->item(previous.row(), 2)->data(Qt::EditRole).toInt();
    //qDebug() << nid << oid;
    emit tableSelecteItemChanged(oid, nid);
}

void TableWidget::onNameEdited(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    qDebug() << "修改了";
    if (topLeft == bottomRight) {
        QStandardItem * item = m_model->itemFromIndex(topLeft);
        //获取对应行
        int row = item->row();
        int id = m_model->item(row, 2)->data(Qt::EditRole).toInt();
        QString name = m_model->item(row, 0)->text();
        QString oName = m_model->item(row, 1)->text();
        QStringList otherName;
        if (oName != "") {
            otherName = oName.split(",");
        }
        emit posNameEdited(id, name, otherName);
    }
}
