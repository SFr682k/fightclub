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


#include "fightclubdashboard.h"
#include <QApplication>
#include <QCommandLineParser>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("Fightclub Dashboard");
    a.setApplicationVersion("0.2");

    QCommandLineParser cmdparser;
    cmdparser.setApplicationDescription("Display the state of multiple Fightclub Departments.");

    cmdparser.addHelpOption();
    cmdparser.addVersionOption();

    QCommandLineOption bmodeappoption(QStringList() << "b" << "batch", "Batch mode. Don't show popups.");
    cmdparser.addOption(bmodeappoption);

    QCommandLineOption fscreenoption(QStringList() << "f" << "fullscreen", "Launch in fullscreen mode.");
    cmdparser.addOption(fscreenoption);

    QCommandLineOption noconfoption("noconfig", "Disable configuration.");
    cmdparser.addOption(noconfoption);


    cmdparser.addPositionalArgument("listofdeps", "[Optional]\nA Fightclub Exchange File containing a list of departments.");


    cmdparser.process(a);
    const QStringList posArgs = cmdparser.positionalArguments();


    bool batchmode = cmdparser.isSet(bmodeappoption);
    bool fscrmode  = cmdparser.isSet(fscreenoption);
    bool noconfig  = cmdparser.isSet(noconfoption);


    FightclubDashboard w;
    w.show();


    if(posArgs.length() > 0) { w.openDepartmentsFile(posArgs.at(0)); }

    if(fscrmode) w.enterFullscreenMode();
    if(noconfig) w.enterNoConfigMode();

    if(!batchmode)              w.openAboutDialog();
    if(!batchmode && !noconfig) w.openSettingsDialog();


    return a.exec();
}
