#include "frameNommerPerso.h"
#include "ui_frameNommerPerso.h"

FrameChoixPerso::FrameChoixPerso(QWidget *parent, unsigned NumJoueur) :
    QFrame(parent),
    ui(new Ui::FrameChoixPerso)
{
    ui->setupUi(this);
    ui->Joueur->setText(NumJoueur);
}

FrameChoixPerso::~FrameChoixPerso()
{
    delete ui;
}
