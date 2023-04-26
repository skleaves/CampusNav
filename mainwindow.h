#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QInputDialog>
#include <QLayout>
#include <QAction>

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

    QAction *normal_action, *add_loc_action, *add_path_action;


public slots:
    void onPrintLog(QString str);
    void onStateChanged(int state);
    void onGetUserInput(bool &isOK, QString &str);
};
#endif // MAINWINDOW_H
