#ifndef QWDIALOGHEADERS_H
#define QWDIALOGHEADERS_H

#include <QDialog>
#include <QStringListModel>
#include <QMessageBox>

#define _T QString::fromLocal8Bit

namespace Ui {
class QWDialogHeaders;
}

class QWDialogHeaders : public QDialog
{
    Q_OBJECT
private:
    QStringListModel  *model;
public:
    explicit QWDialogHeaders(QWidget *parent = nullptr);
    ~QWDialogHeaders();
    void setHeaderList(QStringList& headers);
    QStringList headerList();
private:
    Ui::QWDialogHeaders *ui;
};

#endif // QWDIALOGHEADERS_H
