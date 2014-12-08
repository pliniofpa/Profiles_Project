#-------------------------------------------------
#
# Project created by QtCreator 2014-10-12T05:14:12
#
#-------------------------------------------------

QT       += core gui sql xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

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
    globalconfig.cpp \
    editstylistdialog.cpp \
    editappointmentdialog.cpp \
    edituserdialog.cpp \
    editservicedialog.cpp \
    mycell.cpp \
    myqtablewidget.cpp \
    apptconflitingdialog.cpp \
    richtextdelegate.cpp \
    apptdeleteconfirmationdialog.cpp \
    smtp/smtp.cpp \
    emailconfig.cpp \
    companyconfigdialog.cpp \
    aboutdialog.cpp \
    apptbystylistreportdialog.cpp

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
    globalconfig.h \
    editstylistdialog.h \
    editappointmentdialog.h \
    edituserdialog.h \
    editservicedialog.h \
    mycell.h \
    myqtablewidget.h \
    apptconflitingdialog.h \
    richtextdelegate.h \
    apptdeleteconfirmationdialog.h \
    smtp/smtp.h \
    emailconfig.h \
    companyconfigdialog.h \
    aboutdialog.h \
    apptbystylistreportdialog.h

FORMS    += mainwindow.ui \
    newcustomerdialog.ui \
    newstylistdialog.ui \
    newrecorddialog.ui \
    newservicedialog.ui \
    newappointmentdialog.ui \
    newuserdialog.ui \
    editcustumerdialog.ui \
    editstylistdialog.ui \
    editappointmentdialog.ui \
    edituserdialog.ui \
    editservicedialog.ui \
    apptconflitingdialog.ui \
    apptdeleteconfirmationdialog.ui \
    emailconfig.ui \
    companyconfigdialog.ui \
    aboutdialog.ui \
    apptbystylistreportdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    resources.rc


win32: LIBS += -L$$PWD/KDAB/KDReports-1.5.99/lib/ -lkdreportsd1

INCLUDEPATH += $$PWD/KDAB/KDReports-1.5.99/include
DEPENDPATH += $$PWD/KDAB/KDReports-1.5.99/include
