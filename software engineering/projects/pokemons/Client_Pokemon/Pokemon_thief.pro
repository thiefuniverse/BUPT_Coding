#-------------------------------------------------
#
# Project created by QtCreator 2016-10-22T08:00:23
#
#-------------------------------------------------

QT       += core gui sql widgets network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pokemon_thief
TEMPLATE = app


SOURCES += main.cpp\
    pokemon.cpp \
    player.cpp \
    window_signup.cpp \
    pokemonencrypter.cpp \
    pokemondataprocess.cpp \
    pokemondata.cpp \
    playui.cpp \
    pokemonclient.cpp \
    fightwindow.cpp

HEADERS  += \
    pokemon.h \
    player.h \
    window_signup.h \
    pokemonencrypter.h \
    pokemondataprocess.h \
    pokemondata.h \
    playui.h \
    pokemonclient.h \
    fightwindow.h

FORMS    += \
    window_signup.ui \
    playui.ui \
    fightwindow.ui
RESOURCES += \
    resources.qrc
