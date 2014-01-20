#pragma once

#include <QDialog>

class Edge;

class CostDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CostDialog(QWidget *parent = 0, Edge* _edge = 0);
    ~CostDialog();

private slots:
    void changeCost();
    
private:
    Edge*            edge;
};

