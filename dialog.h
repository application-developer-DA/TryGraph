#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class GraphWidget;

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent, GraphWidget *graph);
    ~Dialog();

private slots:
    void setToOriented();
    void setToNonOriented();

private:
    Ui::Dialog*   ui;
    GraphWidget*  g;
};

#endif // DIALOG_H
