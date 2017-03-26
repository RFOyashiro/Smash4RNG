QT += widgets

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += \
    CCharacter.cpp \
    main.cpp \
    CInitailisation.cpp \
    CMatchs.cpp \
    WinItialisation.cpp \
    WinNommerjoueur.cpp \
    CTournament.cpp

HEADERS += \
    CCharacter.h \
    CInitailisation.h \
    CMatchs.h \
    WinItialisation.h \
    WinNommerjoueur.h \
    CTournament.h

DISTFILES += \
    ressources.rc

RC_FILE += \
    ressources.rc

FORMS += \
    WinItialisation.ui



