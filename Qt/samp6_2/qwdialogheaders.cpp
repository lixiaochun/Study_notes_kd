#include "qwdialogheaders.h"
#include "ui_qwdialogheaders.h"

QWDialogHeaders::QWDialogHeaders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWDialogHeaders)
{
    ui->setupUi(this);
    model= new QStringListModel;
    ui->listView->setModel(model);
}

QWDialogHeaders::~QWDialogHeaders()
{
    QMessageBox::information( this, _T("提示"), _T("设置表头标题对话框被删除") );
    delete ui;
}

void QWDialogHeaders::setHeaderList(QStringList &headers)
{
    model->setStringList(headers);
}

QStringList QWDialogHeaders::headerList()
{
    return model->stringList();
}
