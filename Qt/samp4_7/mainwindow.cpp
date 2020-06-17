#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actListIni_triggered()
{//初始化项
    QListWidgetItem *aItem; //每一行是一个QListWidgetItem

    QIcon aIcon;
    aIcon.addFile(":/images/icons/check2.ico"); //设置ICON的图标
    bool chk=ui->chkBoxListEditable->isChecked();//是否可编辑

    ui->listWidget->clear(); //清除项
    for (int i=0; i<10; i++)
    {
        QString str=QString::asprintf("Item %d",i);
        aItem=new QListWidgetItem(); //新建一个项

        aItem->setText(str); //设置文字标签
        aItem->setIcon(aIcon);//设置图标
        aItem->setCheckState(Qt::Checked); //设置为选中状态
        if (chk) //可编辑, 设置flags
            aItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        else//不可编辑, 设置flags
            aItem->setFlags(Qt::ItemIsSelectable |Qt::ItemIsUserCheckable |Qt::ItemIsEnabled);
        ui->listWidget->addItem(aItem); //增加一个项
    }
}
