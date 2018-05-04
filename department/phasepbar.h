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


#ifndef PHASEPBAR_H
#define PHASEPBAR_H

#include <QObject>
#include <QMessageBox>
#include <QtCore/QTime>
#include <QtCore/QTimer>

class PhasePBar : public QObject
{
    Q_OBJECT
public:
    explicit PhasePBar(QObject *parent = 0);
    ~PhasePBar();

    bool isRunning();
    bool isRoomclock();
    int getElapsedTime();
    int getMaxTime();
    int getLastSavedTime();

private:
    QTime *time;
    QTimer *refreshTimer;
    bool running;
    bool roomclock;
    int savedTime, lastSavedTime;
    int maximumTime;
    int overtime;
    QString timeToString(int);
    QString timeToFlashingString(int);

signals:
    void elapsedTimeUpdate(int);
    void elapsedTimeUpdate(QString);
    void maximumTimeUpdate(QString);
    void phaseProgressUpdate(double);
    void timeRestarted(int);
    void overtimed(int);

private slots:
    void pulse();

public slots:
    void startOrPause();
    void resetTimer();
    void saveCurrentTime();
    void getElapsedOvertime();
    void setElapsedTime(int);
    void setMaximumTime(int);
    void setOvertime(int);
    void setRemainingTime(int);
    void setRoomclock(bool);
};

#endif // PHASEPBAR_H
