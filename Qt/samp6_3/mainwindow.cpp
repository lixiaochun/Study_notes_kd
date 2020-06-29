#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qformdoc.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setVisible(false);
    ui->tabWidget->clear();//清除所有页面
    ui->tabWidget->tabsClosable(); //Page有关闭按钮，可被关闭

    this->setCentralWidget(ui->tabWidget);
    //this->setWindowState(Qt::WindowMaximized); //窗口最大化显示
    this->setAutoFillBackground(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{ //绘制窗口背景图片
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(0,ui->mainToolBar->height(),this->width(),
                       this->height() - ui->mainToolBar->height() - ui->statusBar->height(),
                       QPixmap(_T("./images/images/music.jpg")) );
}

void MainWindow::on_actWidgetInsite_triggered()
{//创建QFormDoc窗体，并在tabWidget中显示
    QFormDoc *formDoc = new QFormDoc(this);
    formDoc->setAttribute (Qt::WA_DeleteOnClose) ; //关闭时自动删除
    int cur=ui->tabWidget->addTab(formDoc,QString::asprintf("Doc %d",ui->tabWidget->count()));
    ui->tabWidget->setCurrentIndex(cur);
    ui->tabWidget->setVisible(true);
}

void MainWindow::on_actWidget_triggered()
{
    QFormDoc *formDoc = new QFormDoc(); //不指定父窗口，用show()显示
    formDoc->setAttribute(Qt::WA_DeleteOnClose); //关闭时自动删除
    formDoc->setWindowTitle(_T("基于QWidget的窗体，无父窗口，关闭时删除"));

    formDoc->setWindowFlag(Qt::Window,true);

    //formDoc->setWindowFlag(Qt::CustomizeWindowHint,true);
    //formDoc->setWindowFlag(Qt::WindowMinMaxButtonsHint,true);
    //formDoc->setWindowFlag(Qt::WindowCloseButtonHint,true);
    //formDoc->setWindowFlag(Qt::WindowStaysOnTopHint,true);

    //formDoc->setWindowState(Qt::WindowMaximized);
    formDoc->setWindowOpacity(0.9);
    //formDoc->setWindowModality(Qt::WindowModal);
    formDoc->show(); //在单独的窗口中显示
}
