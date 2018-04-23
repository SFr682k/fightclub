#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T09:38:36
#
#-------------------------------------------------

QT       += core gui svg network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fightclubclient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    fightclubclient.cpp \
    aboutdialog.cpp \
    phasepbar.cpp \
    settimedialog.cpp \
    listadapter.cpp \
    phaselistmodel.cpp \
    stagelistmodel.cpp \
    broadcastserver.cpp \
    clockwindow.cpp \
    filepropertyparser.cpp \
    problemadapter.cpp \
    problemitemlistmodel.cpp

HEADERS += \
    fightclubclient.h \
    aboutdialog.h \
    phasepbar.h \
    settimedialog.h \
    listadapter.h \
    phaselistmodel.h \
    stagelistmodel.h \
    broadcastserver.h \
    clockwindow.h \
    filepropertyparser.h \
    problemadapter.h \
    problemitemlistmodel.h

FORMS += \
    fightclubclient.ui \
    aboutdialog.ui \
    settimedialog.ui \
    clockwindow.ui

OTHER_FILES += \
    gpl_v3_logo.png

RESOURCES += \
    resources.qrc

include(themeclockwidget.pri)
