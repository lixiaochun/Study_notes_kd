#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QStringList theStrList; //保存初始 StringList
    theStrList<<_T("北京")<<_T("上海")<<_T("天津")<<_T("河北")<<_T("山东")
             <<_T("四川")<<_T("重庆")<<_T("广东")<<_T("河南"); //初始化 StringList
    theModel=new QStringListModel(this); //创建数据模型
    theModel->setStringList(theStrList); //为模型设置StringList，会导入StringList的内容
    ui->listView->setModel(theModel); //为listView设置数据模型
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btnListAppend_clicked()
{//添加一行
    theModel->insertRow(theModel->rowCount()); //在尾部插入一空行
    //QModelIndex  index;
    QModelIndex index=theModel->index(theModel->rowCount()-1,0);//获取最后一行
    theModel->setData(index,"new item",Qt::DisplayRole);//设置显示文字
    ui->listView->setCurrentIndex(index); //设置当前选中的行
}

void Widget::on_btnListInsert_clicked()
{//插入一行
    QModelIndex  index;
    index=ui->listView->currentIndex(); //当前 modelIndex
    theModel->insertRow(index.row()); //在当前行的前面插入一行
    theModel->setData(index,"inserted item",Qt::DisplayRole); //设置显示文字
    theModel->setData(index,Qt::AlignRight,Qt::TextAlignmentRole); //设置对齐方式，不起作用
    ui->listView->setCurrentIndex(index); //设置当前选中的行
}

void Widget::on_btnListDelete_clicked()
{//删除当前行
    QModelIndex  index;
    index=ui->listView->currentIndex(); //获取当前 modelIndex
    theModel->removeRow(index.row()); //删除当前行
}

void Widget::on_btnListClear_clicked()
{//清除ListView的所有项
    theModel->removeRows(0,theModel->rowCount());
}

void Widget::on_btnTextImport_clicked()
{// 显示数据模型的StringList
    QStringList tmpList;
    tmpList=theModel->stringList();//获取数据模型的StringList
    ui->plainTextEdit->clear(); //文本框清空
    for (int i=0; i<tmpList.count();i++)
        ui->plainTextEdit->appendPlainText(tmpList.at(i)); //显示数据模型的StringList()返回的内容
}

void Widget::on_btnPTEditClear_clicked()
{
    ui->plainTextEdit->clear(); //文本框清空
}
