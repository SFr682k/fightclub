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


#include "fightclubnano.h"
#include "ui_fightclubnano.h"

#include "aboutdialog.h"

#include <QTime>

FightclubNano::FightclubNano(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FightclubNano)
{
    ui->setupUi(this);

    ui->controlToolbar->setContextMenuPolicy(Qt::PreventContextMenu);

    if(ui->phaselabel->text().startsWith("<ApplicationName>"))
            ui->phaselabel->setText(QApplication::applicationName().append(", Version ").append(QApplication::applicationVersion()));

    roomclock = true;


    clklgk = new ClockLogic();
    lstadapt = new ListAdapter();


    refreshtimer = new QTimer();


    connect(ui->actionBwd, SIGNAL(triggered(bool)), lstadapt, SLOT(bwd()));
    connect(ui->actionFwd, SIGNAL(triggered(bool)), lstadapt, SLOT(fwd()));

    connect(ui->actionStartPause, SIGNAL(triggered(bool)), clklgk, SLOT(startOrPause()));
    connect(ui->actionStartPause, SIGNAL(triggered(bool)), this, SLOT(toggleStartStopBttn()));

    connect(ui->actionReset, SIGNAL(triggered(bool)), clklgk, SLOT(resetTime()));

    connect(clklgk, SIGNAL(elapsedTimeUpdate(int)), ui->clockwidget, SLOT(setElapsedTime(int)));
    connect(refreshtimer, SIGNAL(timeout()), ui->clockwidget, SLOT(act()));

    connect(clklgk, SIGNAL(elapsedTimeUpdate(QString)), ui->lcdtimedisplay, SLOT(display(QString)));
    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateLCDDisplay()));



    connect(lstadapt, SIGNAL(enablePrevPhaseButton(bool)), ui->actionBwd, SLOT(setEnabled(bool)));
    connect(lstadapt, SIGNAL(enableNextPhaseButton(bool)), ui->actionFwd, SLOT(setEnabled(bool)));


    connect(lstadapt, SIGNAL(resetTime()), clklgk, SLOT(resetTime()));

    connect(lstadapt, SIGNAL(maximumTimeChanged(int)), ui->clockwidget, SLOT(setMaximumTime(int)));

    connect(lstadapt, SIGNAL(phaseLabelChanged(QString)), ui->phaselabel, SLOT(setText(QString)));

    connect(lstadapt, SIGNAL(roomClockChanged(bool)), ui->clockwidget, SLOT(setRoomclock(bool)));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), clklgk, SLOT(setRoomclock(bool)));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), this, SLOT(setRoomclock(bool)));



    lstadapt->setUpPhaseSwitchingButtons();
    refreshtimer->start(30);

    aboutDialogOpen = false;

    lstadapt->loadPhasesListFromFile("foo.fcphases");
}


FightclubNano::~FightclubNano() {
    delete ui;
}


void FightclubNano::openAboutDialog() {
    if(!aboutDialogOpen) {
        aboutDialogOpen = true;
        AboutDialog *ad = new AboutDialog(this);
        ad->exec();
        aboutDialogOpen = false;
    }
}




void FightclubNano::toggleStartStopBttn() {
    if(clklgk->isRunning()) {
        if(clklgk->isRoomclock() && ui->actionFwd->isEnabled()) lstadapt->nextPhase();
        else if(lstadapt->getCurrentPhase() == -1) lstadapt->nextPhase();

        ui->actionStartPause->setIcon(QIcon(":/breeze-icons/chronometer-pause-24.svg"));
    } else ui->actionStartPause->setIcon(QIcon(":/breeze-icons/chronometer-start-24.svg"));
}




void FightclubNano::updateLCDDisplay() {
    if(!roomclock) return;

    QTime now = QTime::currentTime();
    QString displayNow = now.toString(" HH:mm ");
    if((now.second() % 2) != 0) displayNow[3] = ' ';

    ui->lcdtimedisplay->display(displayNow);
}



void FightclubNano::setRoomclock(bool rclk) { roomclock = rclk; }
