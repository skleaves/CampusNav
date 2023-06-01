#ifndef PATHWIDGET_H
#define PATHWIDGET_H

#include <QWidget>

namespace Ui {
class PathWidget;
}

class PathWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PathWidget(QWidget *parent = nullptr);
    ~PathWidget();

private:
    Ui::PathWidget *ui;
};

#endif // PATHWIDGET_H
