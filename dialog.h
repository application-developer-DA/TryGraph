#pragma once

#include <QDialog>

class GraphWidget;

class Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Dialog(QWidget* parent, GraphWidget* graph);
    ~Dialog();

private slots:
    void setToOriented();
    void setToNonOriented();

private:
    GraphWidget*  g;
};

