#pragma once
#include <QMainWindow>

namespace Ui {
class NommerPerso;
}

class NommerPerso : public QMainWindow
{
    Q_OBJECT

public:
    explicit NommerPerso(QWidget *parent = 0,unsigned NbJoueur = 2);
    ~NommerPerso();

private:
    Ui::NommerPerso *ui;
};
