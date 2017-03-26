#ifndef FRAMECHOIXPERSO_H
#define FRAMECHOIXPERSO_H

#include <QFrame>

namespace Ui {
class FrameChoixPerso;
}

class FrameChoixPerso : public QFrame
{
    Q_OBJECT

public:
    explicit FrameChoixPerso(QWidget *parent = 0, unsigned NumJoueur);
    ~FrameChoixPerso();

private:
    Ui::FrameChoixPerso *ui;
};

#endif // FRAMECHOIXPERSO_H
