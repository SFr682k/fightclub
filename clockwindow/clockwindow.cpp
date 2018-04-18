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
#include "ui_clockwindow.h"
#include "broadcastclient.h"



ClockWindow::ClockWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClockWindow)
{   
    ui->setupUi(this);

    if(ui->problabel->text().startsWith("<ApplicationName>"))
        ui->problabel->setText(QApplication::applicationName() + ", Version " + QApplication::applicationVersion());

    if(ui->perflabel->text().startsWith("<empty>"))
        ui->perflabel->setText(" ");


    aboutDialogOpen = false;


    QTimer *acttimer = new QTimer();
    connect(acttimer, SIGNAL(timeout()), ui->clockwidget, SLOT(act()));
    acttimer->start(30);

    refreshtimer = new QTimer();
    refreshtimer->start(30);

    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateTime()));

    roomclock = true;

    BroadcastClient *bcastcli = new BroadcastClient(this, 45454, 12345);
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


ClockWindow::~ClockWindow() {
    delete ui;
}


void ClockWindow::openAboutDialog() {
    if(!aboutDialogOpen) {
        aboutDialogOpen = true;
        AboutDialog *ad = new AboutDialog(this);
        ad->exec();
        aboutDialogOpen = false;
    }
}


void ClockWindow::setPort(uint newport) { emit newPort(newport); }
void ClockWindow::setID(uint newid)     { emit newID(newid); }


void ClockWindow::toggleRoomclock(bool showRClock) {
    if(roomclock && !showRClock)        refreshtimer->stop();
    else if (!roomclock && showRClock)  refreshtimer->start(30);

    roomclock = showRClock;
    ui->clockwidget->setRoomclock(showRClock);
}


void ClockWindow::updateElapsedTime(int elTime) {
    if (roomclock) return;
    QString displayedTime = timeToString(elTime);
    ui->lcdtimedisplay->display(displayedTime);
    ui->lcdtimedisplay->setDigitCount(displayedTime.length());
}


QString ClockWindow::timeToString(int time) {
    QTime temp = QTime(0,0,0,0);
    temp = temp.addMSecs(time);

    QString res;
    if(temp.hour() != 0) res = temp.toString("H:mm:ss");
    else                 res = temp.toString("mm:ss");

    return res;
}


void ClockWindow::updateTime() {
    QTime now = QTime::currentTime();
    QString displayNow = now.toString("HH:mm");
    if((now.second() % 2) != 0) displayNow[2] = ' ';

    ui->lcdtimedisplay->display(displayNow);
    ui->lcdtimedisplay->setDigitCount(displayNow.length());
}



void ClockWindow::resizeEvent(QResizeEvent *event) {
    QFont phaselabelfont = ui->phaselabel->font();
    phaselabelfont.setPointSize(2 + height()*0.03);

    QFont infolabelfont = ui->perflabel->font();
    infolabelfont.setPointSize(4 + height()*0.015);

    ui->phaselabel->setFont(phaselabelfont);
    ui->problabel->setFont(infolabelfont);
    ui->perflabel->setFont(infolabelfont);

    QWidget::resizeEvent(event);
}


void ClockWindow::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_F1:
            openAboutDialog();
            break;

        case Qt::Key_B:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier) {
                // FIXME: Open broadcast client settings
            }
            break;

        case Qt::Key_Q:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                this->close();
            break;

        default:
            QWidget::keyPressEvent(event);
    }
}
