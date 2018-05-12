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

    refreshtimer = new QTimer();


    connect(ui->actionStartPause, SIGNAL(triggered(bool)), clklgk, SLOT(startOrPause()));
    connect(ui->actionStartPause, SIGNAL(triggered(bool)), this, SLOT(toggleStartStopBttn()));

    connect(ui->actionReset, SIGNAL(triggered(bool)), clklgk, SLOT(resetTime()));

    connect(clklgk, SIGNAL(elapsedTimeUpdate(int)), ui->clockwidget, SLOT(setElapsedTime(int)));
    connect(refreshtimer, SIGNAL(timeout()), ui->clockwidget, SLOT(act()));

    connect(clklgk, SIGNAL(elapsedTimeUpdate(QString)), ui->lcdtimedisplay, SLOT(display(QString)));
    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateLCDDisplay()));


    refreshtimer->start(30);

    aboutDialogOpen = false;
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
        /*
        if(clklgk->isRoomclock() && ui->phasefwd->isEnabled()) lstadapt->nextPhase();
        else if((lstadapt->getCurrentStage() == -1) && (lstadapt->getCurrentPhase() == -1))
            lstadapt->nextPhase();
        */

        if(roomclock) {
            roomclock = false;
            ui->clockwidget->setRoomclock(false);
            clklgk->setRoomclock(false);
            ui->clockwidget->setMaximumTime(60000);
        }

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
