#ifndef POSWIDGET_H
#define POSWIDGET_H

#include <QWidget>

namespace Ui {
class PosWidget;
}

class PosWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PosWidget(QWidget *parent = nullptr);
    ~PosWidget();

    void showPosName(QVector<QString> name);

    void setBtnAdd(bool checked);

private:
    Ui::PosWidget *ui;

signals:
    void btnAddToggled(bool checked);
    void lineNameEdited(QVector<QString> name);

public slots:
    void onBtnAdd(bool checked);
    void onLineEditingFinished();
};

#endif // POSWIDGET_H
