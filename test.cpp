#include "test.h"
#include "ui_test.h"

Test::Test(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Test)
{
    ui->setupUi(this);
}

Test::~Test()
{
    delete ui;
}
