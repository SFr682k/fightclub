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


#ifndef CLOCKLOGIC_H
#define CLOCKLOGIC_H

#include <QObject>

#include <QTime>
#include <QTimer>

class ClockLogic : public QObject
{
    Q_OBJECT

public:
    explicit ClockLogic(QObject *parent = nullptr);
    bool isRunning();
    bool isRoomclock();

private:
    QString timeToString(int);

    QTime *time;
    QTimer *refreshTimer;
    int maximumTime, overtime, savedTime;

    bool running, roomclock;

signals:
    void elapsedTimeUpdate(int);
    void elapsedTimeUpdate(QString);
    void overtimed(int);

public slots:
    void startOrPause();
    void resetTime();
    void carryOvertime();
    void setMaximumTime(int);
    void setMaximumOvertime(int);
    void setRoomclock(bool);

    void plusTen();
    void minusTen();

private slots:
    void pulse();
};

#endif // CLOCKLOGIC_H
