#ifndef COSTDIALOG_H
#define COSTDIALOG_H

#include <QDialog>

class Edge;

namespace Ui {
class CostDialog;
}

class CostDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CostDialog(QWidget *parent = 0, Edge* _edge = 0);
    ~CostDialog();

private slots:
    void changeCost();
    
private:
    Ui::CostDialog*  ui;
    Edge*            edge;
};

#endif // COSTDIALOG_H
