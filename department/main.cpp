/****************************************************************************
**
** Copyright (C) 2018 Sebastian Friedl
** Contact: sfr682k@t-online.de
**
** $QT_BEGIN_LICENSE:GPL$
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.md included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $QT_END_LICENSE$
**
****************************************************************************/


#include "fightclubdepartment.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Fightclub Department");
    a.setApplicationVersion("0.6alpha");

    QCommandLineParser cmdparser;
    cmdparser.setApplicationDescription("An enhanced clock system for the IYPT and similar tournaments");

    cmdparser.addHelpOption();
    cmdparser.addVersionOption();

    cmdparser.process(a);

    FightclubDepartment w;
    w.show();

    return a.exec();
}
