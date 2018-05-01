/****************************************************************************
**
** Copyright (C) 2018 Sebastian Friedl
** Contact: sfr682k@t-online.de
**
** Based on the QIYPTClock MultiBroadcastClient
** available on https://github.com/drogenlied/qiyptclock
** Copyright (C) 2011-2012 Jan M. Binder
** Contact: jan.binder@sfz-bw.de
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


#include "signalhelper.h"
#include <QTime>


SignalHelper::SignalHelper(QString t, QObject *parent) :
    QObject(parent)
{
      title = t;

      elapsedTime = -1;
      maximumTime = -1;
      phaseName = "";
      problem = "";
      performers = "";
      roomclock = 2;
}

SignalHelper::~SignalHelper() { }


void SignalHelper::emitSignal(quint32 ntime, quint32 nallowedTime, quint32 nroomclock,
                              QString nphasename, QString nproblem, QString nperformers) {
    if (elapsedTime != (signed) ntime) {
        elapsedTime = ntime;
        emit elapsedTimeUpdate(elapsedTime);
    }

    if (maximumTime != (signed) nallowedTime) {
        maximumTime = nallowedTime;
        emit maximumTimeChanged(maximumTime);
    }

    if (roomclock != nroomclock) {
        roomclock = nroomclock;
        emit roomclockChanged(roomclock > 0);
    }

    if (nphasename != phaseName) {
        phaseName = nphasename;
        emit phaseNameChanged((phaseName != nullptr)? phaseName : " ");
    }

    if(nproblem != problem) {
        problem = nproblem;
        emit problemChanged(problem);
    }

    if(nperformers != performers) {
        performers = nperformers;
        emit performersChanged((performers != nullptr)? performers: " ");
    }
}


QString SignalHelper::getTitle() { return title; }



