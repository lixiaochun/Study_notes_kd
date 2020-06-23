#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>

#define _T QString::fromLocal8Bit

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btnOpen_clicked();

    void on_btnOpenMulti_clicked();

    void on_btnSelectDir_clicked();

    void on_btnSave_clicked();

    void on_btnColor_clicked();

    void on_btnFont_clicked();

    void on_InputString_clicked();

    void on_InputInt_clicked();

    void on_InputFloat_clicked();

    void on_InputItem_clicked();

    void on_btnClear_clicked();

    void on_btnMsgInformation_clicked();

    void on_btnMsgWarning_clicked();

    void on_btnMsgCritical_clicked();

    void on_btnMsgAbout_clicked();

    void on_btnMsgQuestion_clicked();

private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
