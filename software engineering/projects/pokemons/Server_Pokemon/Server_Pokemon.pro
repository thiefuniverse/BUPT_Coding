#-------------------------------------------------
#
# Project created by QtCreator 2016-10-23T22:27:55
#
#-------------------------------------------------

QT       += core gui network sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server_Pokemon
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    player.cpp \
    pokemon.cpp \
    pokemondataprocess.cpp \
    pokemonencrypter.cpp \
    PokemonServer.cpp \
    window_signup.cpp \
    pokemondata.cpp

HEADERS  += mainwindow.h \
    player.h \
    pokemon.h \
    pokemondataprocess.h \
    pokemonencrypter.h \
    PokemonServer.h \
    window_signup.h \
    pokemondata.h \
    orm_mysql.h

FORMS    += mainwindow.ui \
    window_signup.ui

QMAKE_CXXFLAGS += -fpermissive
QMAKE_LIBS+= -lmysqlclient
CONFIG+=C++14

DISTFILES += \
    standardPokemons_config.txt

RESOURCES += \
    resources.qrc
