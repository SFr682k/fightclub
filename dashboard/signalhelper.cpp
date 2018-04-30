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

      time = 0;
      maximumTime = 1;
      stageName = "";
}

SignalHelper::~SignalHelper(){
    qDebug("SignalHelper died");
}

void SignalHelper::fireSignal(quint32 ntime, quint32 nallowedTime, quint32 nroomclock, QString nstageName) {
    if (time != ntime){
        time = ntime;
        emit elapsedTimeUpdate(time);
        emit elapsedTimeUpdate(this->toString());
    }

    if (maximumTime != nallowedTime){
        maximumTime = nallowedTime;
        emit maximumTimeChanged(maximumTime);
    }

    if (roomclock != nroomclock){
        roomclock = nroomclock;
        emit roomclockChanged(roomclock > 0);
    }

    if (nstageName != stageName){
        stageName = nstageName;
        emit phaseNameChanged(stageName);
    }
}


QString SignalHelper::getTitle() {
    return title;
}


QString SignalHelper::toString() {
    QTime tmp = QTime(0,0,0,0);
    tmp = tmp.addMSecs(time);
    QString s = tmp.toString("mm:ss");
    return s;
}
