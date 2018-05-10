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


#ifndef CLIENTBOXWIDGET_H
#define CLIENTBOXWIDGET_H


#include "signalhelper.h"

#include <QGroupBox>
#include <QLabel>
#include <QLCDNumber>
#include <QProgressBar>

class DepartmentBoxWidget : public QGroupBox
{
    Q_OBJECT

public:
    DepartmentBoxWidget(SignalHelper*, QWidget *parent = 0);

private:
    QString clockTitle;

    int elapsedTime, maximumTime;
    bool roomclock;
    QLabel *titlelabel, *perflabel, *phaselabel;
    QProgressBar *progressbar;
    QLCDNumber *timedisplay;

    double fontScale;


private slots:
    void updateElapsedTime(int);
    void updateMaximumTime(int);
    void toggleRoomclock(bool);
    void updateProblem(QString);
    void updateDisplayedTime();
    void updatePhaseProgress();
    QFont getBoxTitleFont(int);
    QFont getPerflabelFont(int);
    QFont getPhaselabelFont(int);
    void setTimeDisplayHeight(int);
    void setProgressBarHeight(int);

public slots:
    void setFontScale(double);
    void onResizeEvent(int);
};

#endif
