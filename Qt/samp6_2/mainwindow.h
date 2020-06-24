#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QAbstractItemView>
#include <QLabel>
#include "qwdialogheaders.h"
#include "qwdialoglocate.h"

#define _T QString::fromLocal8Bit

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QLabel *LabCellPos;    //当前单元格行列号
    QLabel *LabCellText; //当前单元格内容
    QStandardItemModel * theModel; //数据模型
    QItemSelectionModel *theSelection; //选择模型T
private:
    QWDialogHeaders *dlgSetHeaders = NULL;
    QWDialogLocate *dlgLocate=NULL;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actSetRowColumn_triggered();

    void on_actSetHeader_triggered();

    void on_actLocateCell_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
