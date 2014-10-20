#-------------------------------------------------
#
# Project created by QtCreator 2014-10-12T05:14:12
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Profiles_Project
TEMPLATE = app

QMAKE_MAC_SDK=macosx10.9

SOURCES += main.cpp\
        mainwindow.cpp \
    newcustomerdialog.cpp \
    newstylistdialog.cpp

HEADERS  += mainwindow.h \
    newcustomerdialog.h \
    connection.h \
    newstylistdialog.h

FORMS    += mainwindow.ui \
    newcustomerdialog.ui \
    newstylistdialog.ui

RESOURCES += \
    resources.qrc
