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


#include "clocklogic.h"


ClockLogic::ClockLogic(QObject *parent) : QObject(parent) {
    time = new QTime(0,0,0);

    refreshTimer = new QTimer();
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(pulse()));
    refreshTimer->start(10);

    maximumTime = 0, overtime = 0, savedTime = 0;
    running = false, roomclock = true;

}


void ClockLogic::startOrPause() {
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


void ClockLogic::resetTime() {
    savedTime = 0;
    time->restart();
    if(!roomclock) {
        emit elapsedTimeUpdate(time->elapsed());
        emit elapsedTimeUpdate(timeToString(time->elapsed()));
    }

    emit overtimed(-(maximumTime + overtime));
}


void ClockLogic::setMaximumTime(int ms) {
    if(ms >= 0)       maximumTime = ms;
}

void ClockLogic::setMaximumOvertime(int ms) {
    if(ms >= 0) overtime = ms;
}

void ClockLogic::carryOvertime() {
    if(time->elapsed() > maximumTime) *time = time->addMSecs(maximumTime);
    else                              time->restart();
}



void ClockLogic::pulse() {
    if(running && !roomclock) {
        emit elapsedTimeUpdate(time->elapsed());
        emit elapsedTimeUpdate(timeToString(time->elapsed()));

        emit overtimed(time->elapsed() - (maximumTime + overtime));
    }
}



void ClockLogic::setRoomclock(bool rclock) {
    if(!rclock && roomclock) {
        emit elapsedTimeUpdate(0);
        emit elapsedTimeUpdate(timeToString(0));
    }

    roomclock = rclock;
}


void ClockLogic::plusTen() { *time = time->addMSecs(-10000); }

void ClockLogic::minusTen() {
    if(time->elapsed() > 10000) *time = time->addMSecs(10000);
    else                        time->restart();
}


QString ClockLogic::timeToString(int ms) {
    QTime temp = QTime(0,0,0,0);
    temp = temp.addMSecs(ms);

    QString res;
    if(temp.hour() != 0) { res = temp.toString("H:mm:ss"); }
    else                 { res = ' ' + temp.toString("mm:ss") + ' '; }

    return res;
}


bool ClockLogic::isRunning()   { return running; }
bool ClockLogic::isRoomclock() { return roomclock; }
