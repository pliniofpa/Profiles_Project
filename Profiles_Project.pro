#-------------------------------------------------
#
# Project created by QtCreator 2014-10-12T05:14:12
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Profiles_Project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newcustomerdialog.cpp

HEADERS  += mainwindow.h \
    newcustomerdialog.h \
    connection.h

FORMS    += mainwindow.ui \
    newcustomerdialog.ui
