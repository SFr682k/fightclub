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


#include "teleprinter.h"
#include <QApplication>
#include <QCommandLineParser>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Fightclub Teleprinter");
    a.setApplicationVersion("0.10alpha");

    QCommandLineParser cmdparser;
    cmdparser.setApplicationDescription("Clock window for Fightclub Department, designed for use on remote machines.");

    cmdparser.addHelpOption();
    cmdparser.addVersionOption();

    QCommandLineOption portappoption(QStringList() << "p" << "port", "Port to listen on", "port [unsigned int]");
    cmdparser.addOption(portappoption);

    QCommandLineOption idappoption(QStringList() << "i" << "id", "The ID of the Fightclub Department to listen to", "id [unsigned int]");
    cmdparser.addOption(idappoption);

    QCommandLineOption bmodeappoption(QStringList() << "b" << "batch", "Batch mode. Don't show popups.");
    cmdparser.addOption(bmodeappoption);

    QCommandLineOption fscreenoption(QStringList() << "f" << "fullscreen", "Start in fullscreen mode");
    cmdparser.addOption(fscreenoption);

    QCommandLineOption noconfoption("noconfig", "Disable configuration");
    cmdparser.addOption(noconfoption);


    cmdparser.process(a);


    uint port = cmdparser.value(portappoption).toUInt();
    uint id = cmdparser.value(idappoption).toUInt();

    bool batchmode = cmdparser.isSet(bmodeappoption);
    bool fscrmode  = cmdparser.isSet(fscreenoption);
    bool noconfig  = cmdparser.isSet(noconfoption);

    FightclubTeleprinter w;
    w.show();


    if(fscrmode) w.enterFullscreenMode();
    if(noconfig) w.enterNoConfigMode();


    if(port > 0) w.setPort(port);
    if(id > 0)   w.setID(id);

    if(!batchmode)                 w.openAboutDialog();
    if((!(port > 0) || !(id > 0))
       && !batchmode && !noconfig) w.openSettingsDialog();


    return a.exec();
}
