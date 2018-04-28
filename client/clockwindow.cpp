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

#include "aboutdialog.h"


ClockWindow::ClockWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClockWindow)
{   
    ui->setupUi(this);

    if(ui->problabel->text().startsWith("<empty>"))
        ui->problabel->setText(" ");

    if(ui->perflabel->text().startsWith("<empty>"))
        ui->perflabel->setText(" ");


    QTimer *acttimer = new QTimer();
    connect(acttimer, SIGNAL(timeout()), ui->clockwidget, SLOT(act()));
    acttimer->start(30);

    refreshtimer = new QTimer();
    refreshtimer->start(30);

    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateTime()));

    roomclock = false;
    toggleRoomclock(true);
}


ClockWindow::~ClockWindow() {
    delete ui;
}


void ClockWindow::toggleRoomclock(bool showRClock) {
    bool resetTime = false;
    if(roomclock && !showRClock) {
        refreshtimer->stop();
        resetTime = true;
    } else if (!roomclock && showRClock)  refreshtimer->start(30);

    roomclock = showRClock;
    ui->clockwidget->setRoomclock(showRClock);
    if(resetTime) updateElapsedTime(0);
}


void ClockWindow::updateElapsedTime(int elTime) {
    if (roomclock) return;
    ui->clockwidget->setElapsedTime(elTime);
    QString displayedTime = timeToString(elTime);
    ui->lcdtimedisplay->display(displayedTime);
    ui->lcdtimedisplay->setDigitCount(displayedTime.length());
}


void ClockWindow::updateMaximumTime(int maxTime)        { ui->clockwidget->setMaximumTime(maxTime); }
void ClockWindow::phaseNameChanged(QString phaseName)   { ui->phaselabel->setText(phaseName); }
void ClockWindow::problemChanged(QString problem)       { ui->problabel->setText(problem);}
void ClockWindow::performersChanged(QString performers) { ui->perflabel->setText(performers); }


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
        case Qt::Key_F:
            setWindowState(Qt::WindowFullScreen);
            break;

        case Qt::Key_Escape:
            setWindowState(Qt::WindowMaximized);
            break;

        case Qt::Key_Q:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                this->close();
            break;

        default:
            QWidget::keyPressEvent(event);
    }
}


void ClockWindow::closeEvent(QCloseEvent *event) {
    Q_UNUSED(event);
    emit clockwindowClosed();
}


void ClockWindow::kill() { this->close(); }
