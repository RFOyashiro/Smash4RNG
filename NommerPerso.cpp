#include "NommerPerso.h"
#include "ui_ChoisirPerso.h"

ChoisirPerso::ChoisirPerso(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChoisirPerso)
{
    ui->setupUi(this);
}

ChoisirPerso::~ChoisirPerso()
{
    delete ui;
}
