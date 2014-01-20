#include "dialog.h"
#include "graphwidget.h"


Dialog::Dialog(QWidget *parent, GraphWidget *graph) :
    QDialog(parent),
    ui(new Ui::Dialog),
    g(graph)
{
    ui->setupUi(this);
    connect(ui->orientedbtn, SIGNAL(clicked()),
            this, SLOT(setToOriented()));
    connect(ui->nonorientedbtn, SIGNAL(clicked()),
            this, SLOT(setToNonOriented()));
}

Dialog::~Dialog()
{
    delete ui;
    delete g;
}

void Dialog::setToOriented()
{
    g->isOriented = true;
    close();
}

void Dialog::setToNonOriented()
{
    g->isOriented = false;
    close();
}
