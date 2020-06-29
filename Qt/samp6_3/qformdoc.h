#ifndef QFORMDOC_H
#define QFORMDOC_H

#include <QWidget>
#include <QToolBar>
#include <QMessageBox>

#define _T QString::fromLocal8Bit

namespace Ui {
class QFormDoc;
}

class QFormDoc : public QWidget
{
    Q_OBJECT

public:
    explicit QFormDoc(QWidget *parent = nullptr);
    ~QFormDoc();

private:
    Ui::QFormDoc *ui;
};

#endif // QFORMDOC_H
