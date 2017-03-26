#ifndef FRAMENOMMERJOUEUR_H
#define FRAMENOMMERJOUEUR_H

#include <QFrame>

namespace Ui {
class FrameNommerJoueur;
}

class FrameNommerJoueur : public QFrame
{
    Q_OBJECT

public:
    explicit FrameNommerJoueur(QWidget *parent = 0);
    ~FrameNommerJoueur();

private:
    Ui::FrameNommerJoueur *ui;
};

#endif // FRAMENOMMERJOUEUR_H
