#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QLabel>
#include <QDate>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_btnSetHeader_clicked();

    void on_btnIniData_clicked();

private:
    //自定义单元格Type的类型，在创建单元格的item时使用
    enum CellType{ctName=1000,ctSex,ctBirth,ctNation,ctPartyM,ctScore};
    //各字段在表格中的列号
    enum FieldColNum{colName=0,colSex,colBirth,colNation,colScore,colPartyM};
    QLabel *labCellIndex; //状态栏上用于显示单元格的行号、列号
    QLabel *labCellType; //状态栏上用于显示单元格的type
    QLabel *labStudID; //状态栏上用于显示学号
    void createItemsARow(int rowNo,QString Name,QString Sex,QDate birth,QString Nation,bool isPM,int score) ; //为某一行创建 items
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
