#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QPainter>

#define _T QString::fromLocal8Bit

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
    void on_actWidgetInsite_triggered();

    void on_actWidget_triggered();

private:
    void paintEvent(QPaintEvent *event);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
