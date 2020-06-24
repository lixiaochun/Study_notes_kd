#include "qwdialoglocate.h"
#include "ui_qwdialoglocate.h"
#include "mainwindow.h"

QWDialogLocate::QWDialogLocate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWDialogLocate)
{
    ui->setupUi(this);
}

QWDialogLocate::~QWDialogLocate()
{
    delete ui;
}

void QWDialogLocate::on_btnSetText_clicked()
{//定位到单元格，并设置字符串
    int row=ui->spinBoxRow->value(); //行号
    int col=ui->spinBoxColumn->value();//列号

    MainWindow *parWind = (MainWindow*)parentWidget(); //获取主窗口
    parWind->setACellText(row,col,ui->edtCaption->text()); //设置单元格文字
    if (ui->chkBoxRow->isChecked()) //行增
        ui->spinBoxRow->setValue(1+ui->spinBoxRow->value());

    if (ui->chkBoxColumn->isChecked()) //列增
        ui->spinBoxColumn->setValue(1+ui->spinBoxColumn->value());
}
