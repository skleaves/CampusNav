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

    QMap<QString, int> nameToId;

private:
    Ui::FindPathWidget *ui;
    QStringList str;



public slots:
    void onPushBtnFindClicked();
    void onPushBtnClearClicked();
    void onEditTextChanged(QString str);

signals:
    void pushBtnFindPressed(int start, int end);
    void pushBtnClearPressed();

};

#endif // FINDPATHWIDGET_H
