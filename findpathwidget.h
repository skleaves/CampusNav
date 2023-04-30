#ifndef FINDPATHWIDGET_H
#define FINDPATHWIDGET_H

#include <QWidget>
#include <QCompleter>
#include <QListView>

namespace Ui {
class FindPathWidget;
}

class FindPathWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FindPathWidget(QWidget *parent = nullptr);
    ~FindPathWidget();

    void loadItems(QStringList str);

private:
    Ui::FindPathWidget *ui;
};

#endif // FINDPATHWIDGET_H
