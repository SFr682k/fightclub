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


#include "phasepbar.h"

#include <QDebug>

PhasePBar::PhasePBar(QObject *parent) : QObject(parent) {
    time = new QTime;
    refreshTimer = new QTimer;

    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(pulse()));

    running = false;
    roomclock = true;
    savedTime = 0;
    maximumTime = 1;
    overtime = 0;

    refreshTimer->start(10);
}


PhasePBar::~PhasePBar() {

}


void PhasePBar::startOrPause() {
    if(running) {
        savedTime = time->elapsed();
        running = false;
    } else {
        time->start();
        *time = time->addMSecs(-savedTime);
        running = true;
    }

    if(!roomclock) {
        emit elapsedTimeUpdate(time->elapsed());
        emit elapsedTimeUpdate(timeToString(time->elapsed()));
    }
}


void PhasePBar::resetTimer() {
    savedTime = 0;
    time->restart();
    emit timeRestarted(0);
    if(!roomclock) {
        emit elapsedTimeUpdate(time->elapsed());
        emit elapsedTimeUpdate(timeToString(0));
    }
    emit(overtimed(-(maximumTime + overtime + 25))); // +25 to avoid inaccuracies
    emit phaseProgressUpdate(0);
}


void PhasePBar::setElapsedTime(int elapsedms) {
    time->start();
    *time = time->addMSecs(-elapsedms);
    savedTime = elapsedms;

    emit elapsedTimeUpdate(elapsedms);
    emit elapsedTimeUpdate(this->timeToString(elapsedms));
    emit overtimed(time->elapsed() - (maximumTime + overtime));
    if(!roomclock) emit phaseProgressUpdate(savedTime*1.0/maximumTime);
}


void PhasePBar::setMaximumTime(int maximumms) {
    if(!(maximumms > 0)) return;

    maximumTime = maximumms;
    if(!roomclock) emit maximumTimeUpdate(timeToString(maximumTime));
}

void PhasePBar::setRemainingTime(int remainingms) {
    if(remainingms > maximumTime) {
        time->restart();
        savedTime = 0;
    } else {
        time->start();
        *time = time->addMSecs(-(maximumTime - remainingms));
        savedTime = maximumTime - remainingms;
    }

    emit elapsedTimeUpdate(time->elapsed());
    emit elapsedTimeUpdate(timeToString(time->elapsed()));
    emit overtimed(time->elapsed() - (maximumTime + overtime));
    emit phaseProgressUpdate(time->elapsed()*1.0/maximumTime);

    if(remainingms > maximumTime)
        QMessageBox::information(0, "Value too large",
            "The specified remaining time is larger than the current phase's maximum duration. Elapsed time has been set to 00:00.");
}

void PhasePBar::setOvertime(int overtimems) {
    if(overtimems >= 0) {
        overtime = overtimems;
        if((time->elapsed() < 50) && (maximumTime + overtime <= 60000))
            emit(overtimed(-(maximumTime + overtime + 25))); // +25 to avoid inaccuracies
        else if(time->elapsed() >= (maximumTime + overtime -60000))
            emit overtimed(time->elapsed() - (maximumTime + overtime));
    }
}

void PhasePBar::getElapsedOvertime() {
    int elapsedOverTime = time->elapsed() - maximumTime;
    time->restart();
    if(elapsedOverTime > 0) *time = time->addMSecs(-elapsedOverTime);

    emit elapsedTimeUpdate(time->elapsed());
    emit elapsedTimeUpdate(timeToString(time->elapsed()));
    emit phaseProgressUpdate(time->elapsed()*1.0/maximumTime);
}


void PhasePBar::setRoomclock(bool rclock) {
    if(!rclock && roomclock) {
        emit elapsedTimeUpdate(0);
        emit elapsedTimeUpdate(timeToString(0));
    }

    roomclock = rclock;
    if(roomclock) {
        emit elapsedTimeUpdate(" --:-- ");
        emit phaseProgressUpdate(0);
        emit maximumTimeUpdate(" --:-- ");
    }
}


void PhasePBar::pulse() {
    if(running && !roomclock) {
        emit elapsedTimeUpdate(time->elapsed());
        emit elapsedTimeUpdate(timeToString(time->elapsed()));
        emit phaseProgressUpdate((time->elapsed()*1.0/maximumTime));

        if(time->elapsed() >= (maximumTime + overtime -60000))
            emit overtimed(time->elapsed() - (maximumTime + overtime));
    } else if(savedTime != 0 && !roomclock) {
        emit elapsedTimeUpdate(timeToFlashingString(time->elapsed()));
    }
}




QString PhasePBar::timeToString(int ms) {
    QTime temp = QTime(0,0,0,0);
    temp = temp.addMSecs(ms);

    QString res;
    if(temp.hour() != 0) { res = temp.toString("H:mm:ss"); }
    else                 { res = ' ' + temp.toString("mm:ss") + ' '; }

    return res;
}


QString PhasePBar::timeToFlashingString(int ms) {
    if((ms - savedTime) % 1000 < 500) {
        QTime temp = QTime(0,0,0,0);
        temp = temp.addMSecs(savedTime);
        if(temp.hour() != 0) { return " :  :  "; }
        else                 { return "   :   "; }
    } else { return this->timeToString(savedTime); }
}




bool PhasePBar::isRunning()   { return running; }
bool PhasePBar::isRoomclock() { return roomclock; }
int  PhasePBar::getMaxTime()  { return maximumTime; }
