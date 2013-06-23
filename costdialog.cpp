#include "costdialog.h"
#include "ui_costdialog.h"

#include "edge.h"

CostDialog::CostDialog(QWidget *parent, Edge *_edge) :
    QDialog(parent),
    ui(new Ui::CostDialog),
    edge(_edge)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(changeCost()));
}

CostDialog::~CostDialog()
{
    delete ui;
    //delete edge;
}

void CostDialog::changeCost()
{
    edge->setCost(ui->lineEdit->text().toInt());
}
