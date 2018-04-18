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


#include "clockwindow.h"
#include <QApplication>
#include <QCommandLineParser>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Fightclub Client Clock Window");
    a.setApplicationVersion("0.5");

    QCommandLineParser cmdparser;
    cmdparser.setApplicationDescription("Clock window for the Fightclub Client. May be used on remote machines.");

    cmdparser.addHelpOption();
    cmdparser.addVersionOption();

    QCommandLineOption portappoption(QStringList() << "p" << "port", "Port to listen on", "port [unsigned int]");
    cmdparser.addOption(portappoption);

    QCommandLineOption idappoption(QStringList() << "i" << "id", "The ID of the FightclubClient to listen to", "id [unsigned int]");
    cmdparser.addOption(idappoption);

    cmdparser.process(a);


    uint port = cmdparser.value(portappoption).toUInt();
    uint id = cmdparser.value(idappoption).toUInt();

    ClockWindow w;
    w.show();

    if(port > 0) w.setPort(port);
    if(id > 0)   w.setID(id);

    w.openAboutDialog();
    w.openSetupBCastDialog();

    return a.exec();
}
