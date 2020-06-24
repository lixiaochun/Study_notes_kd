#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qwdialogsize.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    theModel = new QStandardItemModel (2, 6, this) ; //数据模型
    theSelection = new QItemSelectionModel (theModel) ;//选择模型
    connect(theSelection,SIGNAL(currentChanged(QModelIndex,QModelIndex)), this,SLOT(on_currentChanged(QModelIndex,QModelIndex)));
    ui->tableView->setModel (theModel) ; //设置数据模型
    ui->tableView->setSelectionModel(theSelection) ; //设置选择模型
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    //创建状态栏组件
    LabCellPos = new QLabel(_T("当前单元格："), this);
    LabCellPos->setMinimumWidth(200);
    LabCellText = new QLabel(_T("单元格内容："), this);
    LabCellText->setMinimumWidth(200);

    ui->statusbar->addWidget(LabCellPos);
    ui->statusbar->addWidget(LabCellText);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actSetRowColumn_triggered()
{ //模态对话框，动态创建，用过后删除
    QWDialogSize *dlgTableSize=new QWDialogSize(this);
    Qt::WindowFlags flags=dlgTableSize->windowFlags();
    dlgTableSize->setWindowFlags(flags | Qt::MSWindowsFixedSizeDialogHint);
    dlgTableSize->setRowColumn(theModel->rowCount(),theModel->columnCount ());
    int ret=dlgTableSize->exec () ;// 以模态方式显示对话框
    if (ret==QDialog::Accepted)
    { //OK按钮被按下，获取对话框上的输入，设置行数和列数
        int cols=dlgTableSize->columnCount();
        theModel->setColumnCount(cols);
        int rows=dlgTableSize->rowCount();
        theModel->setRowCount(rows);
    }
    delete dlgTableSize;
}

void MainWindow::on_actSetHeader_triggered()
{//一次创建，多次调用，对话框关闭时只是隐藏
    if (dlgSetHeaders==NULL) //如果对象没有被创建过，就创建对象
        dlgSetHeaders = new QWDialogHeaders(this);

    if (dlgSetHeaders->headerList().count()!=theModel->columnCount())
    {//如果表头列数变化，重新初始化
        QStringList strList;
        for (int i=0;i<theModel->columnCount();i++)//获取现有的表头标题
            strList.append(theModel->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
        dlgSetHeaders->setHeaderList(strList);//用于对话框初始化显示
    }

    int ret=dlgSetHeaders->exec();// 以模态方式显示对话框
    if (ret==QDialog::Accepted) //OK键被按下
    {
        QStringList strList=dlgSetHeaders->headerList();//获取对话框上修改后的StringList
        theModel->setHorizontalHeaderLabels(strList);// 设置模型的表头标题
    }
}

void MainWindow::on_actLocateCell_triggered()
{//创建 StayOnTop的对话框，对话框关闭时自动删除
    //通过控制actTab_Locate的enable属性避免重复点击
    ui->actLocateCell->setEnabled(false);
    dlgLocate = new QWDialogLocate(this); //创建对话框，传递指针
    dlgLocate->setAttribute(Qt::WA_DeleteOnClose); //对话框关闭时自动删除对话框对象,用于不需要读取返回值的对话框
    Qt::WindowFlags flags=dlgLocate->windowFlags(); //获取已有flags
    dlgLocate->setWindowFlags(flags | Qt::WindowStaysOnTopHint); //设置对话框固定大小,StayOnTop
    //对话框初始化设置
    dlgLocate->setSpinRange(theModel->rowCount(),theModel->columnCount());
    QModelIndex curIndex=theSelection->currentIndex();
    if (curIndex.isValid())
       dlgLocate->setSpinValue(curIndex.row(),curIndex.column());
    dlgLocate->show(); //非模态显示对话框
}
