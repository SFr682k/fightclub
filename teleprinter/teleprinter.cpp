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
#include "ui_teleprinter.h"

#include "aboutdialog.h"
#include "setupbroadcastdialog.h"


FightclubTeleprinter::FightclubTeleprinter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FightclubTeleprinter)
{      
    ui->setupUi(this);

    bcastcli = new BroadcastClient(this);
    setupbcastdial = new SetupBroadcastDialog(this);



    if(ui->problabel->text().startsWith("<ApplicationName>"))
        ui->problabel->setText(QApplication::applicationName() + ", Version " + QApplication::applicationVersion());

    if(ui->perflabel->text().startsWith("<empty>"))
        ui->perflabel->setText(" ");


    aboutDialogOpen = false;
    bcastSettingsOpen = false;


    QTimer *acttimer = new QTimer();
    connect(acttimer, SIGNAL(timeout()), ui->clockwidget, SLOT(act()));
    acttimer->start(30);

    refreshtimer = new QTimer();
    refreshtimer->start(30);

    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateTime()));

    roomclock = true;


    connect(bcastcli, SIGNAL(elapsedTimeUpdate(int)), ui->clockwidget, SLOT(setElapsedTime(int)));
    connect(bcastcli, SIGNAL(elapsedTimeUpdate(int)), this, SLOT(updateElapsedTime(int)));

    connect(bcastcli, SIGNAL(maximumTimeChanged(int)), ui->clockwidget, SLOT(setMaximumTime(int)));
    connect(bcastcli, SIGNAL(roomClockChanged(bool)), this, SLOT(toggleRoomclock(bool)));

    connect(bcastcli, SIGNAL(phaseNameChanged(QString)), ui->phaselabel, SLOT(setText(QString)));
    connect(bcastcli, SIGNAL(problemChanged(QString)), ui->problabel, SLOT(setText(QString)));
    connect(bcastcli, SIGNAL(performersChanged(QString)), ui->perflabel, SLOT(setText(QString)));

    connect(this, SIGNAL(newPort(uint)), bcastcli, SLOT(setListeningPort(uint)));
    connect(this, SIGNAL(newID(uint)), bcastcli, SLOT(setSignature(uint)));

    ui->clockwidget->setRoomclock(true);
}


FightclubTeleprinter::~FightclubTeleprinter() {
    delete ui;
}


void FightclubTeleprinter::openAboutDialog() {
    if(!aboutDialogOpen) {
        aboutDialogOpen = true;
        AboutDialog *ad = new AboutDialog(this);
        ad->exec();
        aboutDialogOpen = false;
    }
}


void FightclubTeleprinter::openSetupBCastDialog() {
    if(setupbcastdial->exec()) {
        emit newPort(setupbcastdial->getBroadcastPort());
        emit newID(setupbcastdial->getBroadcastID());
    }
}


void FightclubTeleprinter::setPort(uint newport) {
    setupbcastdial->setPort(newport);
    emit newPort(newport);
}

void FightclubTeleprinter::setID(uint newid) {
    setupbcastdial->setID(newid);
    emit newID(newid);
}


uint FightclubTeleprinter::getBcastPort() { return bcastcli->getBcastPort(); }
uint FightclubTeleprinter::getBcastID()   { return bcastcli->getBcastSignature(); }


void FightclubTeleprinter::toggleRoomclock(bool showRClock) {
    if(roomclock && !showRClock)        refreshtimer->stop();
    else if (!roomclock && showRClock)  refreshtimer->start(30);

    roomclock = showRClock;
    ui->clockwidget->setRoomclock(showRClock);
}


void FightclubTeleprinter::updateElapsedTime(int elTime) {
    if (roomclock) return;
    QString displayedTime = timeToString(elTime);
    ui->lcdtimedisplay->display(displayedTime);
    ui->lcdtimedisplay->setDigitCount(displayedTime.length());
}


QString FightclubTeleprinter::timeToString(int time) {
    QTime temp = QTime(0,0,0,0);
    temp = temp.addMSecs(time);

    QString res;
    if(temp.hour() != 0) res = temp.toString("H:mm:ss");
    else                 res = temp.toString("mm:ss");

    return res;
}


void FightclubTeleprinter::updateTime() {
    QTime now = QTime::currentTime();
    QString displayNow = now.toString("HH:mm");
    if((now.second() % 2) != 0) displayNow[2] = ' ';

    ui->lcdtimedisplay->display(displayNow);
    ui->lcdtimedisplay->setDigitCount(displayNow.length());
}



void FightclubTeleprinter::resizeEvent(QResizeEvent *event) {
    QFont phaselabelfont = ui->phaselabel->font();
    phaselabelfont.setPointSize(2 + height()*0.03);

    QFont infolabelfont = ui->perflabel->font();
    infolabelfont.setPointSize(4 + height()*0.015);

    ui->phaselabel->setFont(phaselabelfont);
    ui->problabel->setFont(infolabelfont);
    ui->perflabel->setFont(infolabelfont);

    QWidget::resizeEvent(event);
}


void FightclubTeleprinter::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_F1:
            openAboutDialog();
            break;

        case Qt::Key_B:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                openSetupBCastDialog();
            break;

        case Qt::Key_F:
            setWindowState(Qt::WindowFullScreen);
            break;

        case Qt::Key_Escape:
            if(windowState() == Qt::WindowFullScreen) setWindowState(Qt::WindowMaximized);
            break;

        case Qt::Key_Q:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                this->close();
            break;

        default:
            QWidget::keyPressEvent(event);
    }
}
