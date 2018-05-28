#-------------------------------------------------
#
# Copyright (C) 2018 Sebastian Friedl
# Contact: sfr682k@t-online.de
#
# $QT_BEGIN_LICENSE:GPL$
# GNU General Public License Usage
# This file may be used under the terms of the GNU
# General Public License version 3.0 as published by the Free Software
# Foundation and appearing in the file LICENSE.md included in the
# packaging of this file.  Please review the following information to
# ensure the GNU General Public License version 3.0 requirements will be
# met: http://www.gnu.org/copyleft/gpl.html.
# $QT_END_LICENSE$
#
#-------------------------------------------------


QT       += core gui network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fightclub-dashboard
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
    fightclubdashboard.cpp \
    aboutdialog.cpp \
    multibroadcastclient.cpp \
    signalhelper.cpp \
    sockethelper.cpp \
    settingsdialog.cpp \
    filepropertyparser.cpp

HEADERS += \
    fightclubdashboard.h \
    aboutdialog.h \
    multibroadcastclient.h \
    signalhelper.h \
    sockethelper.h \
    settingsdialog.h \
    filepropertyparser.h

FORMS += \
    fightclubdashboard.ui \
    aboutdialog.ui \
    settingsdialog.ui

RESOURCES += \
    resources.qrc

include(departmentboxwidget.pri)

