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


#include "clientboxwidget.h"

#include <QGridLayout>
#include <QTime>

#include <QDebug>

ClientBoxWidget::ClientBoxWidget(SignalHelper *sigHelp, QWidget *parent) :
    QGroupBox(parent)
{
    clockTitle = sigHelp->getTitle();

    elapsedTime = 0;
    maximumTime = 1;
    roomclock = true;

    setTitle(clockTitle);
    setFont(getBoxTitleFont(parent->height()));

    QGridLayout *root = new QGridLayout();
    root->setSpacing(0);
    root->setColumnStretch(0,9);
    root->setColumnStretch(1,1);
    root->setRowStretch(0,2);
    root->setRowStretch(1,2);
    root->setRowStretch(2,1);
    setLayout(root);

    perflabel = new QLabel(" ");
    perflabel->setAlignment(Qt::AlignBottom);
    perflabel->setWordWrap(true);
    perflabel->setFont(getPerflabelFont(parent->height()));
    root->addWidget(perflabel,0,0);

    phaselabel = new QLabel("Waiting for a broadcast");
    phaselabel->setAlignment(Qt::AlignTop);
    phaselabel->setWordWrap(true);
    phaselabel->setFont(getPhaselabelFont(parent->height()));
    root->addWidget(phaselabel,1,0);


    timedisplay = new QLCDNumber();
    timedisplay->setDigitCount(7);
    timedisplay->display("  --:--");
    setTimeDisplayHeight(parent->height());
    timedisplay->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    timedisplay->setFrameStyle(QFrame::NoFrame);
    root->addWidget(timedisplay,0,1,2,1);


    // The progress bar has to be packed into an extra layout
    // to optain a margin on the top of it
    QGridLayout *progresslayout = new QGridLayout();
    progresslayout->setContentsMargins(0, 8, 0, 0);
    root->addLayout(progresslayout,2,0,1,2);

    progressbar = new QProgressBar();
    progressbar->setTextVisible(false);
    progressbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setProgressBarHeight(parent->height());
    progresslayout->addWidget(progressbar,0,0);



    connect(sigHelp, SIGNAL(elapsedTimeUpdate(int)), this, SLOT(updateElapsedTime(int)));
    connect(sigHelp, SIGNAL(elapsedTimeUpdate(int)), this, SLOT(updateDisplayedTime()));
    connect(sigHelp, SIGNAL(elapsedTimeUpdate(int)), this, SLOT(updatePhaseProgress()));

    connect(sigHelp, SIGNAL(maximumTimeChanged(int)), this, SLOT(updateMaximumTime(int)));
    connect(sigHelp, SIGNAL(maximumTimeChanged(int)), this, SLOT(updatePhaseProgress()));

    connect(sigHelp, SIGNAL(roomclockChanged(bool)), this, SLOT(toggleRoomclock(bool)));
    connect(sigHelp, SIGNAL(roomclockChanged(bool)), this, SLOT(updateDisplayedTime()));
    connect(sigHelp, SIGNAL(roomclockChanged(bool)), this, SLOT(updatePhaseProgress()));

    connect(sigHelp, SIGNAL(phaseNameChanged(QString)), phaselabel, SLOT(setText(QString)));
    connect(sigHelp, SIGNAL(performersChanged(QString)), perflabel, SLOT(setText(QString)));
    connect(sigHelp, SIGNAL(problemChanged(QString)), this, SLOT(updateProblem(QString)));

    updatePhaseProgress();
}




void ClientBoxWidget::updateElapsedTime(int elapsed) { elapsedTime = elapsed; }
void ClientBoxWidget::updateMaximumTime(int maxTime) { if(maxTime != 0) maximumTime = maxTime; }
void ClientBoxWidget::toggleRoomclock(bool roomclck) { roomclock   = roomclck; }


void ClientBoxWidget::updateProblem(QString problem) {
    if((problem != " ") && (problem != "") && (problem != nullptr))
        setTitle(clockTitle + "  —  " + problem);
    else setTitle(clockTitle);
}



void ClientBoxWidget::updateDisplayedTime() {
    QTime tmp = QTime(0,0,0,0);
    tmp = tmp.addMSecs(elapsedTime);
    QString timestr;

    if(!roomclock) {
        if(tmp.hour() > 0) timestr = tmp.toString("H:mm:ss");
        else               timestr = "  " + tmp.toString("mm:ss");
    } else timestr = "  --:--";

    timedisplay->display(timestr);
}


void ClientBoxWidget::updatePhaseProgress() {
    double progress;

    if(!roomclock) progress = elapsedTime*1.0/maximumTime*1.0;
    else           progress = 0;

    progressbar->setMaximum(2000);

    QString lowerbborder;
    QString upperbborder;

    if(((int) (progress*2000.0) % 2000) == 0) {
        lowerbborder = "0";
        upperbborder = "0.00001";
    } else {
        lowerbborder = QString::number(((int) (progress*2000.0) % 2000)/2000.0 - 0.00001);
        upperbborder = QString::number(((int) (progress*2000.0) % 2000)/2000.0);
    }

    if(progress < 0.75)      progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: #32c81e;}");
    else if(progress <= 1.0) progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: #ffbe1e;}");
    else if(progress <= 2.0) progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: QLinearGradient(x1: 0,y1: 0,x2: 1, y2: 0, stop: 0 #c80000, stop: " + lowerbborder + " #c80000, stop: " + upperbborder + " #ffbe1e, stop: 1 #ffbe1e);}");
    else if(progress <= 3.0) progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: QLinearGradient(x1: 0,y1: 0,x2: 1, y2: 0, stop: 0 #641e64, stop: " + lowerbborder + " #641e64, stop: " + upperbborder + " #c80000, stop: 1 #c80000);}");
    else if(progress > 3.0)  progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: #641e64;}");

    if(progress <= 1.0) progressbar->setValue(((int) (progress*2000)) % 2000);
    else                progressbar->setValue(2000);
}



QFont ClientBoxWidget::getBoxTitleFont(int windowHeight) {
    QFont tmp = font();
    tmp.setPointSize(windowHeight*0.019);
    return tmp;
}

QFont ClientBoxWidget::getPerflabelFont(int windowHeight) {
    QFont tmp = font();
    tmp.setPointSize(windowHeight*0.019);
    return tmp;
}

QFont ClientBoxWidget::getPhaselabelFont(int windowHeight) {
    QFont tmp = font();
    tmp.setPointSize(windowHeight*0.02);
    tmp.setBold(true);
    return tmp;
}


void ClientBoxWidget::onResizeEvent(int windowHeight) {
    this->setFont(this->getBoxTitleFont(windowHeight));
    perflabel->setFont(getPerflabelFont(windowHeight));
    phaselabel->setFont(getPhaselabelFont(windowHeight));
    setTimeDisplayHeight(windowHeight);
    setProgressBarHeight(windowHeight);
}


void ClientBoxWidget::setTimeDisplayHeight(int windowHeight) {
    timedisplay->setMinimumHeight(windowHeight*0.045);
    timedisplay->setMaximumHeight(windowHeight*0.045);
}


void ClientBoxWidget::setProgressBarHeight(int windowHeight) {
    progressbar->setMinimumHeight(windowHeight*0.015);
    progressbar->setMaximumHeight(windowHeight*0.015);
}
