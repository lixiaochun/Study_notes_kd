#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QFileDialog>
#include <QTextStream>

#define _T QString::fromLocal8Bit

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QLabel *LabCurFile;    //当前文件
    QLabel *LabCellPos;    //当前单元格行列号
    QLabel *LabCellText; //当前单元格内容
    QStandardItemModel * theModel; //数据模型
    QItemSelectionModel *theSelection; //选择模型
    void iniModelFromStringList (QStringList&) ; //从 StringList 初始化数据模型

private slots:
    //当前选择单元格发生变化
    void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_actOpen_triggered();

    void on_actSave_triggered();

    void on_actAppend_triggered();

    void on_actInsert_triggered();

    void on_actDelete_triggered();

    void on_acrAlignLeft_triggered();

    void on_actAligntCenter_triggered();

    void on_actAligntRight_triggered();

    void on_actFontBold_triggered(bool checked);

    void on_actModleData_triggered();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    int FixedColumnCount;
};
#endif // MAINWINDOW_H
