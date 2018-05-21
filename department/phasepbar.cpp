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



PhasePBar::PhasePBar(QObject *parent) : QObject(parent) {
    time = new QTime;
    refreshTimer = new QTimer;

    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(pulse()));

    running = false;
    roomclock = true;
    savedTime = 0;
    lastSavedTime = -1;
    lastAutosavedTime = -1;
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


void PhasePBar::saveCurrentTime() {
    if(running) lastSavedTime = time->elapsed();
    else        lastSavedTime = savedTime;
}


void PhasePBar::timePlus10() {
    *time = time->addMSecs(-10000);
    savedTime += 10000;

    if(!roomclock) {
        emit elapsedTimeUpdate(running? time->elapsed() : savedTime);
        pulse();
        emit overtimed((running? time->elapsed() : savedTime) - (maximumTime + overtime));
        emit phaseProgressUpdate((running? time->elapsed() : savedTime)*1.0/maximumTime);
    }
}

void PhasePBar::timeMinus10() {
    if(time->elapsed() > 10000) *time = time->addMSecs(10000);
    else                        time->restart();

    if(savedTime > 10000) savedTime -= 10000;
    else                  savedTime  = 0;

    if(!roomclock) {
        emit elapsedTimeUpdate(running? time->elapsed() : savedTime);
        pulse();
        emit overtimed((running? time->elapsed() : savedTime) - (maximumTime + overtime));
        emit phaseProgressUpdate((running? time->elapsed() : savedTime)*1.0/maximumTime);
    }
}


void PhasePBar::setElapsedTime(int elapsedms) {
    time->start();
    *time = time->addMSecs(-elapsedms);
    savedTime = elapsedms;

    emit elapsedTimeUpdate(elapsedms);
    pulse();
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
    pulse();
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

        if(time->elapsed() > 15000) {
            lastAutosavedTime = time->elapsed();
            emit lastAutosavedTimeUpdate(lastAutosavedTime);
        }

        if(time->elapsed() >= (maximumTime + overtime -60000))
            emit overtimed(time->elapsed() - (maximumTime + overtime));
    } else if(!roomclock) {
        emit elapsedTimeUpdate(timeToFlashingString(time->elapsed()));
        if((savedTime > 15000) && savedTime != lastAutosavedTime) {
            lastAutosavedTime = savedTime;
            emit lastAutosavedTimeUpdate(lastAutosavedTime);
        }
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
    if(((ms - savedTime) % 1000 < 500) && (savedTime > 0)) {
        QTime temp = QTime(0,0,0,0);
        temp = temp.addMSecs(savedTime);
        if(temp.hour() != 0) { return " :  :  "; }
        else                 { return "   :   "; }
    } else { return this->timeToString(savedTime); }
}




bool PhasePBar::isRunning()        { return running; }
bool PhasePBar::isRoomclock()      { return roomclock; }

int  PhasePBar::getElapsedTime()   {
    if(running) return time->elapsed();
    else        return savedTime;
}

int  PhasePBar::getMaxTime()       { return maximumTime; }
int  PhasePBar::getLastSavedTime() { return lastSavedTime; }
