TEMPLATE = app

QT += gui sql widgets
CONFIG += c++11 console

SOURCES += main.cpp \
    logfile.cpp \
    strategy.cpp \
    planchange.cpp \
    timetable.cpp \
    traveller.cpp \
    createnewcustomer.cpp \
    mainwindow.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    logfile.h \
    strategy.h \
    planchange.h \
    timetable.h \
    traveller.h \
    createnewcustomer.h \
    mainwindow.h

FORMS += \
    createnewcustomer.ui \
    mainwindow.ui

