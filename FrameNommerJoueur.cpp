#include "FrameNommerJoueur.h"
#include "ui_FrameNommerJoueur.h"

FrameNommerJoueur::FrameNommerJoueur(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameNommerJoueur)
{
    ui->setupUi(this);
}

FrameNommerJoueur::~FrameNommerJoueur()
{
    delete ui;
}
