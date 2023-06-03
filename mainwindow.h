#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QInputDialog>
#include <QLayout>
#include <QAction>
#include <QLabel>
#include <QCompleter>
#include <QListView>
#include <QTableWidget>
#include "map.h"
#include "findpathwidget.h"
#include "poswidget.h"
#include "pathwidget.h"
#include "tablewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    FindPathWidget *findPathWidget;
    PosWidget *posWidget;
    PathWidget *pathWidget;
    TableWidget *tableWidget;

    QLabel *currentPos;
    QAction *normal_action, *add_loc_action, *add_path_action;




public slots:
    void onActionNormal(bool checked);
    void onActionPos(bool checked);
    void onActionPath(bool checked);
    void onPosChanged();
    void onStateChanged(int state);
    void onGetUserInput(bool &isOK, QString &str);
    void onShowSelectedPos(Pos * pos);
    void onlineNameEdited(QString name, QStringList otherName);
    void onPosNameEdited(int id, QString name, QStringList otherName);
    void onShowTableWidget();
    void onDelSelectedPos();

    void onPushBtnFindPressed(int start, int end);
    void onPushBtnClearPressed();
    void onPrintLog(QString msg);

};
#endif // MAINWINDOW_H
