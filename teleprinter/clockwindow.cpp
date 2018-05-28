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



ClockWindow::ClockWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClockWindow)
{      
    ui->setupUi(this);
    QMainWindow::setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    ui->problabel->setMouseTracking(true);
    ui->perflabel->setMouseTracking(true);
    ui->phaselabel->setMouseTracking(true);
    connect(ui->clockwidget, SIGNAL(mouseMoved()), this, SLOT(cursorMoved()));
    ui->lcdtimedisplay->setMouseTracking(true);


    if(ui->problabel->text().startsWith("<ApplicationName>"))
        ui->problabel->setText(QApplication::applicationName() + ", Version " + QApplication::applicationVersion());

    if(ui->perflabel->text().startsWith("<empty>"))
        ui->perflabel->setText(" ");


    defaultFont = font();
    fontScale = 1;


    QTimer *acttimer = new QTimer();
    connect(acttimer, SIGNAL(timeout()), ui->clockwidget, SLOT(act()));
    acttimer->start(16);

    refreshtimer = new QTimer();
    refreshtimer->start(33);

    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateTime()));

    roomclock = true;

    ui->clockwidget->setRoomclock(true);
}


ClockWindow::~ClockWindow() {
    delete ui;
    delete refreshtimer;
    this->deleteLater();
}



void ClockWindow::phaseNameChanged(QString phasename) { ui->phaselabel->setText(phasename); }

void ClockWindow::updateElapsedTime(int elTime) {
    if (roomclock) return;
    QString displayedTime = timeToString(elTime);

    ui->clockwidget->setElapsedTime(elTime);
    ui->lcdtimedisplay->display(displayedTime);
    ui->lcdtimedisplay->setDigitCount(displayedTime.length());
}

void ClockWindow::updateMaximumTime(int maxtime)        { ui->clockwidget->setMaximumTime(maxtime); }
void ClockWindow::problemChanged(QString problem)       { ui->problabel->setText(problem); }
void ClockWindow::performersChanged(QString performers) { ui->perflabel->setText(performers); }

void ClockWindow::toggleRoomclock(bool showRClock) {
    if(roomclock && !showRClock)        refreshtimer->stop();
    else if (!roomclock && showRClock)  refreshtimer->start(30);

    roomclock = showRClock;
    ui->clockwidget->setRoomclock(showRClock);
}



void ClockWindow::showRclockSecondHand(bool show) { ui->clockwidget->showSecondHand(show); }
void ClockWindow::setRclockBehavior(int behavior) { ui->clockwidget->setRoomclockMode(behavior); }








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








void ClockWindow::setWindowFont(QString family) { this->setFont((family == nullptr)? defaultFont : QFont(family)); }

void ClockWindow::setFontScale(double newScale) {
    if(newScale > 0.5) fontScale = newScale;
    resizeEvent(new QResizeEvent(QSize(width(),height()),QSize(width(),height())));
}









void ClockWindow::resizeEvent(QResizeEvent *event) {
    QFont phaselabelfont = ui->phaselabel->font();
    phaselabelfont.setPointSize((2 + height()*0.03)*fontScale);

    QFont infolabelfont = ui->perflabel->font();
    infolabelfont.setPointSize((4 + height()*0.015)*fontScale);

    ui->phaselabel->setFont(phaselabelfont);
    ui->problabel->setFont(infolabelfont);
    ui->perflabel->setFont(infolabelfont);

    QWidget::resizeEvent(event);
}


void ClockWindow::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_F:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier) {
                setWindowState(Qt::WindowFullScreen);
                cursorMoved();
            }
            break;

        case Qt::Key_Escape:
            if(windowState() == Qt::WindowFullScreen) {
                setWindowState(Qt::WindowMaximized);
                cursorMoved();
            }
            break;

        case Qt::Key_Q:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                this->close();
            break;

        default:
            QWidget::keyPressEvent(event);
    }
}


void ClockWindow::mouseMoveEvent(QMouseEvent *event) {
    cursorMoved();
    QWidget::mouseMoveEvent(event);
}


void ClockWindow::cursorMoved() { emit cursorPosChanged(windowState() == Qt::WindowFullScreen); }

