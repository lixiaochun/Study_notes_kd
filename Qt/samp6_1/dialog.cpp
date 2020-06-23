#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_btnOpen_clicked()
{//选择单个文件
    QString curPath=QDir::currentPath();//获取系统当前目录
    //获取应用程序的路径
    QString dlgTitle = _T("选择一个文件"); //对话框标题
    QString filter= _T("文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)"); //文件过滤器
    QString aFileName = QFileDialog::getOpenFileName(this, dlgTitle, curPath, filter);
    if (!aFileName.isEmpty())
        ui->plainTextEdit->appendPlainText(aFileName);
}

void Dialog::on_btnOpenMulti_clicked()
{//选择多个文件
    //获取应用程序的路径
    QString curPath=QDir::currentPath();//获取系统当前目录
    QString dlgTitle = _T("选择多个文件"); //对话框标题
    QString filter = _T("文本文件(*.txt);;图片文件(*.jpg *.gif *.png);;所有文件(*.*)"); //文件过滤器

    QStringList fileList = QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);
    for (int i=0; i<fileList.count();i++)
        ui->plainTextEdit->appendPlainText(fileList.at(i));
}

void Dialog::on_btnSelectDir_clicked()
{//选择文件夹
    QString curPath=QCoreApplication::applicationDirPath();//获取系统当前目录
    //获取应用程序的路径
    QString dlgTitle = _T("选择一个文件夹"); //对话框标题
    QString aFileName = QFileDialog::getExistingDirectory(this, dlgTitle, curPath, QFileDialog::ShowDirsOnly);
    if (!aFileName.isEmpty())
        ui->plainTextEdit->appendPlainText(aFileName);
}

void Dialog::on_btnSave_clicked()
{//保存文件
    QString curPath=QCoreApplication::applicationDirPath(); //获取应用程序的路径
    QString dlgTitle = _T("保存文件"); //对话框标题
    QString filter = _T("文本文件(*.txt);;h文件(*.h);;C++文件(.cpp);;所有文件(*.*)"); //文件过滤器
    QString aFileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if (!aFileName.isEmpty())
        ui->plainTextEdit->appendPlainText(aFileName);
}

void Dialog::on_btnColor_clicked()
{
    QPalette pal=ui->plainTextEdit->palette(); //获取现有 palette
    QColor  iniColor=pal.color(QPalette::Text); //现有的文字颜色
    QColor color=QColorDialog::getColor(iniColor,this, _T("选择颜色"));
    if (color.isValid()) //选择有效
    {
        pal.setColor(QPalette::Text,color); //palette 设置选择的颜色
        ui->plainTextEdit->setPalette(pal); //设置 palette
    }
}

void Dialog::on_btnFont_clicked()
{//选择字体
    QFont iniFont=ui->plainTextEdit->font(); //获取文本框的字体
    bool   ok=false;
    QFont font=QFontDialog::getFont(&ok,iniFont); //选择字体
    if (ok) //选择有效
        ui->plainTextEdit->setFont(font);
}

void Dialog::on_InputString_clicked()
{//输入字符串
    QString dlgTitle = _T("输入文字对话框");
    QString txtLabel = _T("请输入文件名");
    QString defaultInput = _T("新建文件.txt");
    QLineEdit::EchoMode echoMode = QLineEdit::Normal;//正常文字输入
    //QLineEdit::EchoMode echoMode=QLineEdit::Password;//密码输入
    bool ok=false;
    QString text = QInputDialog::getText(this, dlgTitle, txtLabel, echoMode, defaultInput, &ok);
    if (ok && !text.isEmpty())
        ui->plainTextEdit->appendPlainText(text);
}

void Dialog::on_InputInt_clicked()
{//输入整数
    QString dlgTitle = _T("输入整数对话框");
    QString txtLabel = _T("设置字体大小");
    int defaultValue=ui->plainTextEdit->font().pointSize(); //现有字体大小
    int minValue = 6, maxValue = 50, stepValue = 1; //范围，步长
    bool ok=false;
    int inputValue = QInputDialog::getInt(this, dlgTitle,txtLabel,
                               defaultValue, minValue,maxValue,stepValue,&ok);
    if (ok) //是否确认输入
    {
        QFont font=ui->plainTextEdit->font();
        font.setPointSize(inputValue);
        ui->plainTextEdit->setFont(font);
    }
}

void Dialog::on_InputFloat_clicked()
{//输入浮点数
    QString dlgTitle = _T("输入浮点数对话框");
    QString txtLabel = _T("输入一个浮点数");
    float defaultValue = 3.13;

    float minValue=0, maxValue=10000;  //范围
    int decimals= 2;//小数点位数

    bool ok=false;
    float inputValue = QInputDialog::getDouble(this, dlgTitle,txtLabel,
                            defaultValue, minValue,maxValue,decimals,&ok);
    if (ok) //确认选择
    {
        QString str= _T("输入了一个浮点数:") + QString::asprintf("%.2f",inputValue);
        ui->plainTextEdit->appendPlainText(str);
    }
}

void Dialog::on_InputItem_clicked()
{//条目选择输入
    QStringList items; //ComboBox 列表的内容
    items << _T("优秀") << _T("良好") << _T("合格") << _T("不合格");

    QString dlgTitle = _T("条目选择对话框");
    QString txtLabel = _T("请选择级别");
    int     curIndex=0; //初始选择项
    bool    editable=true; //ComboBox是否可编辑
    bool    ok=false;
    QString text = QInputDialog::getItem(this, dlgTitle,txtLabel,items,curIndex,editable,&ok);

    if (ok && !text.isEmpty())
        ui->plainTextEdit->appendPlainText(text);
}

void Dialog::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}

void Dialog::on_btnMsgInformation_clicked()
{
    QString dlgTitle = _T("information消息框");
    QString strInfo = _T("文件已经打开，字体大小已设置");
    QMessageBox::information(this, dlgTitle, strInfo,
                             QMessageBox::Ok,QMessageBox::NoButton);
}

void Dialog::on_btnMsgWarning_clicked()
{
    QString dlgTitle = _T("warning 消息框");
    QString strInfo = _T("文件内容已经被修改");
    QMessageBox::warning(this, dlgTitle, strInfo);
}

void Dialog::on_btnMsgCritical_clicked()
{
    QString dlgTitle = _T("critical消息框");
    QString strInfo = _T("有不明程序访问网络");
    QMessageBox::critical(this, dlgTitle, strInfo);
}

void Dialog::on_btnMsgAbout_clicked()
{
    QString dlgTitle = _T("about消息框");
    QString strInfo = _T("我开发的数据查看软件 V1.0 \n 保留所有版权");
    QMessageBox::about(this, dlgTitle, strInfo);
}

void Dialog::on_btnMsgQuestion_clicked()
{
    QString dlgTitle = _T("Question消息框");
    QString strInfo = _T("文件已被修改，是否保存修改？");

    QMessageBox::StandardButton  defaultBtn = QMessageBox::NoButton; //缺省按钮

    QMessageBox::StandardButton result;//返回选择的按钮
    result=QMessageBox::question(this, dlgTitle, strInfo,
                      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                      defaultBtn);

    if (result==QMessageBox::Yes)
        ui->plainTextEdit->appendPlainText( _T("Question消息框: Yes 被选择") );
    else if(result==QMessageBox::No)
        ui->plainTextEdit->appendPlainText( _T("Question消息框: No 被选择" ) );
    else if(result==QMessageBox::Cancel)
        ui->plainTextEdit->appendPlainText( _T("Question消息框: Cancel 被选择") );
    else
        ui->plainTextEdit->appendPlainText( _T("Question消息框: 无选择") );
}
