#include "pathwidget.h"
#include "ui_pathwidget.h"

PathWidget::PathWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathWidget)
{
    ui->setupUi(this);

    ui->textBrowser->setFont(QFont("方正标雅宋_GBK", 12));
    connect(ui->textBrowser, &QTextBrowser::textChanged, this, &PathWidget::scrollToEnd);
    //ui->textBrowser->setTextColor(Qt::darkGray);
    //ui->textBrowser->append(GREEN_TEXT("进入添加路径模式，请单击以设置起点"));
    //ui->textBrowser->append(" <font color = gray>""进入添加路径模式，请单击以设置起点""</font> ");
    //ui->textBrowser->append("");
    //QString idStr = "19";
    //ui->textBrowser->insertHtml(GRAY_TEXT("起点设置成功") " <font color=green>" + idStr + "</font> " " <font color=gray></font> ");
}

PathWidget::~PathWidget()
{
    delete ui;
}

void PathWidget::clearText()
{
    ui->textBrowser->clearHistory();
    ui->textBrowser->clear();
}

void PathWidget::printMsg(QString msg)
{
    ui->textBrowser->insertHtml(msg);
}

void PathWidget::scrollToEnd()
{
    ui->textBrowser->moveCursor(QTextCursor::End);
}
