#include "WinItialisation.h"
#include <QMainWindow>
#include <QWindow>
#include <iostream>
#include <ui_WinItialisation.h>
#include "WinNommerJoueur.h"
#include "WinNommerjoueur.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::on_Submit_clicked()
{
    std::cout << ui->NbJoueur->value() << std::endl;
    std::cout << ui->TypeMatch->currentText().toStdString() << std::endl;    
    std::cout << ui->NommerJoueur2->isChecked() << std::endl;
    std::cout << ui->ChoisirPerso2->isChecked() << std::endl;
    WinNommerJoueur(ui->ChoisirPerso2->isChecked(), ui->NbJoueur->value());
    return 0;
}



