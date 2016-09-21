#-------------------------------------------------
#
# Project created by QtCreator 2016-09-14T08:21:45
#
#-------------------------------------------------

QT       += core gui webkitwidgets xml network
QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


#DEPENDPATH += . ../QtDropbox
INCLUDEPATH += ../QtDropbox/src

LIBS += -L../QtDropbox/ -lQtDropbox

TARGET = MPire
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    requestdropboxdirlist.cpp

HEADERS  += mainwindow.h \
    requestdropboxdirlist.h

FORMS    += mainwindow.ui
