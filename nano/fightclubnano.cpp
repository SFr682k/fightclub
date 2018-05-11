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


    refreshtimer = new QTimer();

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




void FightclubNano::updateLCDDisplay() {
    if(!roomclock) return;

    QTime now = QTime::currentTime();
    QString displayNow = now.toString("HH:mm");
    if((now.second() % 2) != 0) displayNow[2] = ' ';

    ui->lcdtimedisplay->display(displayNow);
    ui->lcdtimedisplay->setDigitCount(displayNow.length());
}
