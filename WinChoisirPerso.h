/*#ifndef WINCHOISIRPERSO_H
#define WINCHOISIRPERSO_H

#include <QMainWindow>

namespace Ui {
class WinChoisirPerso;
}

class WinChoisirPerso : public QMainWindow
{
    Q_OBJECT

public:
    explicit WinChoisirPerso(unsigned NbJoueur = 2, QWidget *parent = 0);
    ~WinChoisirPerso();

private:
    Ui::WinChoisirPerso *ui;
    unsigned m_NbJoueur;
};

#endif // WINCHOISIRPERSO_H*/
