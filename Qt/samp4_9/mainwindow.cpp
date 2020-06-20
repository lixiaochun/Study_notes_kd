#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitterMain);
    //状态栏初始化创建
    labCellIndex = new QLabel ("当前单元格坐标：", this);
    labCellIndex->setMinimumWidth(250);
    labCellType=new QLabel ("当前单元格类型：",this);
    labCellType->setMinimumWidth(200);
    labStudID=new QLabel ("学生ID: ", this);
    labStudID->setMinimumWidth(200);
    ui->statusbar->addWidget (labCellIndex) ; //加到状态栏
    ui->statusbar->addWidget(labCellType);
    ui->statusbar->addWidget(labStudID);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnSetHeader_clicked()
{ //设置表头
    QTableWidgetItem    *headerItem;
    QStringList headerText;
    headerText<<"姓名 "<<"性别 "<<"出生日期 "<<"民族 "<<"分数 "<<"是否党员 ";
    //ui->tableInfo->setHorizontalHeaderLabels(headerText);
    ui->tableInfo->setColumnCount(headerText.count());//列数设置为与 headerText的行数相等
    for (int i=0;i<ui->tableInfo->columnCount();i++)//列编号从0开始
    {
        //cellItem=ui->tableInfo->horizontalHeaderItem(i);
        headerItem=new QTableWidgetItem(headerText.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
        QFont font=headerItem->font();//获取原有字体设置
        font.setBold(true);//设置为粗体
        font.setPointSize(12);//字体大小
        headerItem->setTextColor(Qt::red);//字体颜色
        headerItem->setFont(font);//设置字体
        ui->tableInfo->setHorizontalHeaderItem(i,headerItem); //设置表头单元格的Item
    }

    //ui->tableInfo->setItemDelegateForColumn(colScore,&spinDelegate);//设置自定义代理组件
}

void MainWindow::on_btnIniData_clicked()
{//初始化表格内容
    QString strName,strSex;
    bool    isParty=false;

    QDate   birth;
    birth.setDate(1980,4,7);//初始化一个日期
    ui->tableInfo->clearContents();//只清除工作区，不清除表头

    int Rows=ui->tableInfo->rowCount(); //数据区行数，
    for (int i=0;i<Rows;i++) //数据区第1行的编号为0，所以范围是0~rowCount()-1
    {
        strName=QString::asprintf("学生%d",i); //学生姓名
        if ((i % 2)==0) //分奇数，偶数行设置性别，及其图标
            strSex="男 ";
        else
            strSex="女 ";

        createItemsARow(i, strName, strSex, birth,"汉族 ",isParty,70); //为某一行创建items

        birth=birth.addDays(20); //日期加20天
        isParty =!isParty;
    }
}

void MainWindow::createItemsARow(int rowNo,QString Name,QString Sex,QDate birth,QString Nation,bool isPM,int score)
{ //为一行的单元格创建 Items
    QTableWidgetItem    *item;
    QString str;
    uint StudID=201605000; //学号基数

    //姓名
    //新建一个Item，设置单元格type为自定义的MainWindow::ctName
    item=new  QTableWidgetItem(Name,MainWindow::ctName);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter); //文本对齐格式
    StudID  +=rowNo; //学号=基数+ 行号
    item->setData(Qt::UserRole,QVariant(StudID));  //设置studID为data
    ui->tableInfo->setItem(rowNo,MainWindow::colName,item); //为单元格设置Item

    //性别
    QIcon   icon;
    if (Sex=="男 ")
        icon.addFile(":/images/icons/boy.ico");
    else
        icon.addFile(":/images/icons/girl.ico");
    item=new  QTableWidgetItem(Sex,MainWindow::ctSex); //新建一个Item，设置单元格type为自定义的 MainWindow::ctSex
    item->setIcon(icon);
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//为单元格设置Item
    ui->tableInfo->setItem(rowNo,MainWindow::colSex,item);//为单元格设置Item

    //出生日期
    str=birth.toString("yyyy-MM-dd"); //日期转换为字符串
    item=new  QTableWidgetItem(str,MainWindow::ctBirth);//新建一个Item，设置单元格type为自定义的 MainWindow::ctBirth
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter); //文本对齐格式
    ui->tableInfo->setItem(rowNo,MainWindow::colBirth,item);//为单元格设置Item

    //民族
    item=new  QTableWidgetItem(Nation,MainWindow::ctNation); //新建一个Item，设置单元格type为自定义的 MainWindow::ctNation
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//文本对齐格式
    ui->tableInfo->setItem(rowNo,MainWindow::colNation,item);//为单元格设置Item

    //是否党员
    item=new  QTableWidgetItem("党员",MainWindow::ctPartyM);//新建一个Item，设置单元格type为自定义的 MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//文本对齐格式
    if (isPM)
        item->setCheckState(Qt::Checked);
    else
        item->setCheckState(Qt::Unchecked);
    item->setBackgroundColor(Qt::yellow);//Qt::green  lightGray  yellow
    ui->tableInfo->setItem(rowNo,MainWindow::colPartyM,item);//为单元格设置Item

    //分数
    str.setNum(score);
    item=new  QTableWidgetItem(str,MainWindow::ctScore);//新建一个Item，设置单元格type为自定义的 MainWindow::ctPartyM
    item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);//文本对齐格式
    ui->tableInfo->setItem(rowNo,MainWindow::colScore,item);//为单元格设置Item
}
