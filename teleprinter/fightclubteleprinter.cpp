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


#include "fightclubteleprinter.h"
#include "ui_fightclubteleprinter.h"

#include "aboutdialog.h"
#include "clockwindow.h"


FightclubTeleprinter::FightclubTeleprinter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FightclubTeleprinter) {
    ui->setupUi(this);



    ui->apptitle->setText(QApplication::applicationName());
    ui->versionlabel->setText("Version " + QApplication::applicationVersion());

    QString platformName;
    if(QApplication::platformName() == "xcb")          platformName = "Linux (X11)";
    else if(QApplication::platformName() == "windows") platformName = "Windows";
    else if(QApplication::platformName() == "cocoa")   platformName = "macOS (Cocoa)";
    else                                               platformName = QApplication::platformName();

    ui->qtadvertizing->setText(QString("Using Qt ") + qVersion() + QString(" on ") + platformName);



    bcastcli = new BroadcastClient(this);
    settingsdial = new TeleprinterSettings(this);

    aboutDialogOpen = false, settingsDialogOpen = false;



    cachedPhaseName = "Waiting for a broadcast", cachedProblem = " ", cachedPerformers = " ";
    cachedElapsedTime = 0, cachedMaximumTime = 1;
    cachedRoomclock = true;

    connect(this, SIGNAL(newPort(uint)), bcastcli, SLOT(setListeningPort(uint)));
    connect(this, SIGNAL(newID(uint)), bcastcli, SLOT(setSignature(uint)));


    connect(ui->openClockWindow, SIGNAL(clicked(bool)), this, SLOT(openClockWindow()));
    connect(ui->closeAllClockWindows, SIGNAL(clicked(bool)), this, SLOT(purgeClockWindows()));
    connect(ui->openAboutDialog, SIGNAL(clicked(bool)), this, SLOT(openAboutDialog()));
    connect(ui->openSettingsDialog, SIGNAL(clicked(bool)), this, SLOT(openSettingsDialog()));
}


FightclubTeleprinter::~FightclubTeleprinter() {
    delete ui;
}


void FightclubTeleprinter::openAboutDialog() {
    if(!aboutDialogOpen) {
        aboutDialogOpen = true;
        //cursorMoved();
        AboutDialog *ad = new AboutDialog(this);
        ad->exec();
        aboutDialogOpen = false;
        //cursorMoved();
    }
}


void FightclubTeleprinter::openSettingsDialog() {
    if(!settingsDialogOpen) {
        settingsDialogOpen = true;
        //cursorMoved();
        if(settingsdial->exec()) {
            emit newPort(settingsdial->getBroadcastPort());
            emit newID(settingsdial->getBroadcastID());
        }
        settingsDialogOpen = false;
        //cursorMoved();
    }
}


void FightclubTeleprinter::setPort(uint newport) {
    settingsdial->setPort(newport);
    emit newPort(newport);
}

void FightclubTeleprinter::setID(uint newid) {
    settingsdial->setID(newid);
    emit newID(newid);
}


uint FightclubTeleprinter::getBcastPort() { return bcastcli->getBcastPort(); }
uint FightclubTeleprinter::getBcastID()   { return bcastcli->getBcastSignature(); }




void FightclubTeleprinter::openClockWindow() {
    ClockWindow *clkwindow = new ClockWindow();

    connect(bcastcli, SIGNAL(phaseNameChanged(QString)), clkwindow, SLOT(phaseNameChanged(QString)));
    connect(bcastcli, SIGNAL(elapsedTimeUpdate(int)), clkwindow, SLOT(updateElapsedTime(int)));
    connect(bcastcli, SIGNAL(maximumTimeChanged(int)), clkwindow, SLOT(updateMaximumTime(int)));
    connect(bcastcli, SIGNAL(roomClockChanged(bool)), clkwindow, SLOT(toggleRoomclock(bool)));
    connect(bcastcli, SIGNAL(problemChanged(QString)), clkwindow, SLOT(problemChanged(QString)));
    connect(bcastcli, SIGNAL(performersChanged(QString)), clkwindow, SLOT(performersChanged(QString)));

    connect(settingsdial, SIGNAL(fontChanged(QString)), clkwindow, SLOT(setWindowFont(QString)));
    connect(settingsdial, SIGNAL(fontScaleChanged(double)), clkwindow, SLOT(setFontScale(double)));
    connect(settingsdial, SIGNAL(showRclockSecondHand(bool)), clkwindow, SLOT(showRclockSecondHand(bool)));
    connect(settingsdial, SIGNAL(rclockBehaviorChanged(int)), clkwindow, SLOT(setRclockBehavior(int)));

    clkwindow->phaseNameChanged(cachedPhaseName);
    clkwindow->updateElapsedTime(cachedElapsedTime);
    clkwindow->updateMaximumTime(cachedMaximumTime);
    clkwindow->toggleRoomclock(cachedRoomclock);

    clkwindow->problemChanged(cachedProblem);
    clkwindow->performersChanged(cachedPerformers);

    clkwindow->setWindowFont(settingsdial->getFontFamily());
    clkwindow->setFontScale(settingsdial->getFontScale());
    clkwindow->showRclockSecondHand(settingsdial->getShowRClkSecondHand());
    clkwindow->setRclockBehavior(settingsdial->getRClkBehavior());

    connect(this, SIGNAL(closeAllClockWindows()), clkwindow, SLOT(close()));

    clkwindow->show();
}


void FightclubTeleprinter::purgeClockWindows() { emit closeAllClockWindows(); }


void FightclubTeleprinter::cachePhaseName(QString phasename)   { cachedPhaseName = phasename; }
void FightclubTeleprinter::cacheElapsedTime(int elapsedTime)   { cachedElapsedTime = elapsedTime; }
void FightclubTeleprinter::cacheMaximumTime(int maximumTime)   { cachedMaximumTime = maximumTime; }
void FightclubTeleprinter::cacheRoomclock(bool roomclock)      { cachedRoomclock = roomclock; }
void FightclubTeleprinter::cacheProblem(QString problem)       { cachedProblem = problem; }
void FightclubTeleprinter::cachePerformers(QString performers) { cachedPerformers = performers; }



void FightclubTeleprinter::enterFullscreenMode() {
    //setWindowState(Qt::WindowFullScreen);
    //cursorMoved();
}

void FightclubTeleprinter::enterNoConfigMode()   { settingsdial->enterNoConfigMode(); }
