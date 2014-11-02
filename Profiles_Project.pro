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
ICON = app.icns

win32:RC_FILE = resources.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    newcustomerdialog.cpp \
    newstylistdialog.cpp \
    mydatamodel.cpp \
    newrecorddialog.cpp \
    newservicedialog.cpp \
    newappointmentdialog.cpp \
    newuserdialog.cpp \
    editcustumerdialog.cpp \
    globalconfig.cpp

HEADERS  += mainwindow.h \
    newcustomerdialog.h \
    connection.h \
    newstylistdialog.h \
    mydatamodel.h \
    newrecorddialog.h \
    newservicedialog.h \
    newappointmentdialog.h \
    newuserdialog.h \
    version.h \
    editcustumerdialog.h \
    globalconfig.h

FORMS    += mainwindow.ui \
    newcustomerdialog.ui \
    newstylistdialog.ui \
    newrecorddialog.ui \
    newservicedialog.ui \
    newappointmentdialog.ui \
    newuserdialog.ui \
    editcustumerdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    resources.rc
