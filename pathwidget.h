#ifndef PATHWIDGET_H
#define PATHWIDGET_H

#include <QWidget>


#define GRAY_TEXT(STRING) " <font color=gray>" STRING "</font> " " <font color=gray></font> "
#define BLUE_TEXT(STRING) " <font color=blue>" STRING "</font> " " <font color=gray></font> "
#define GREEN_TEXT(STRING) " <font color=green>" STRING "</font> " " <font color=gray></font> "


namespace Ui {
class PathWidget;
}

class PathWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PathWidget(QWidget *parent = nullptr);
    ~PathWidget();

    void clearText();
    void printMsg(QString msg);

private:
    Ui::PathWidget *ui;

public slots:
    void scrollToEnd();

};

#endif // PATHWIDGET_H
