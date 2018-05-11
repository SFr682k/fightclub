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


#include "teleprinter.h"
#include "ui_teleprinter.h"

#include "aboutdialog.h"
#include "teleprintersettings.h"



FightclubTeleprinter::FightclubTeleprinter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FightclubTeleprinter)
{      
    ui->setupUi(this);
    QMainWindow::setMouseTracking(true);
    ui->centralWidget->setMouseTracking(true);
    ui->problabel->setMouseTracking(true);
    ui->perflabel->setMouseTracking(true);
    ui->phaselabel->setMouseTracking(true);
    connect(ui->clockwidget, SIGNAL(mouseMoved()), this, SLOT(cursorMoved()));
    ui->lcdtimedisplay->setMouseTracking(true);

    hideCursorTimer = new QTimer();
    hideCursorTimer->setInterval(5000);
    connect(hideCursorTimer, SIGNAL(timeout()), this, SLOT(hideCursor()));


    bcastcli = new BroadcastClient(this);
    settingsdial = new TeleprinterSettings(this);


    if(ui->problabel->text().startsWith("<ApplicationName>"))
        ui->problabel->setText(QApplication::applicationName() + ", Version " + QApplication::applicationVersion());

    if(ui->perflabel->text().startsWith("<empty>"))
        ui->perflabel->setText(" ");


    aboutDialogOpen = false, settingsDialogOpen = false;

    QTimer *acttimer = new QTimer();
    connect(acttimer, SIGNAL(timeout()), ui->clockwidget, SLOT(act()));
    acttimer->start(30);

    refreshtimer = new QTimer();
    refreshtimer->start(30);

    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateTime()));

    roomclock = true;

    defaultFont = font();
    fontScale = 1;

    connect(bcastcli, SIGNAL(elapsedTimeUpdate(int)), ui->clockwidget, SLOT(setElapsedTime(int)));
    connect(bcastcli, SIGNAL(elapsedTimeUpdate(int)), this, SLOT(updateElapsedTime(int)));

    connect(bcastcli, SIGNAL(maximumTimeChanged(int)), ui->clockwidget, SLOT(setMaximumTime(int)));
    connect(bcastcli, SIGNAL(roomClockChanged(bool)), this, SLOT(toggleRoomclock(bool)));

    connect(bcastcli, SIGNAL(phaseNameChanged(QString)), ui->phaselabel, SLOT(setText(QString)));
    connect(bcastcli, SIGNAL(problemChanged(QString)), ui->problabel, SLOT(setText(QString)));
    connect(bcastcli, SIGNAL(performersChanged(QString)), ui->perflabel, SLOT(setText(QString)));

    connect(this, SIGNAL(newPort(uint)), bcastcli, SLOT(setListeningPort(uint)));
    connect(this, SIGNAL(newID(uint)), bcastcli, SLOT(setSignature(uint)));

    connect(settingsdial, SIGNAL(fontChanged(QString)), this, SLOT(setWindowFont(QString)));
    connect(settingsdial, SIGNAL(fontScaleChanged(double)), this, SLOT(setFontScale(double)));

    connect(settingsdial, SIGNAL(showRclockSecondHand(bool)), ui->clockwidget, SLOT(showSecondHand(bool)));
    connect(settingsdial, SIGNAL(rclockBehaviorChanged(int)), ui->clockwidget, SLOT(setRoomclockMode(int)));

    ui->clockwidget->setRoomclock(true);
}


FightclubTeleprinter::~FightclubTeleprinter() {
    delete ui;
}


void FightclubTeleprinter::openAboutDialog() {
    if(!aboutDialogOpen) {
        aboutDialogOpen = true;
        cursorMoved();
        AboutDialog *ad = new AboutDialog(this);
        ad->exec();
        aboutDialogOpen = false;
        cursorMoved();
    }
}


void FightclubTeleprinter::openSettingsDialog() {
    if(!settingsDialogOpen) {
        settingsDialogOpen = true;
        cursorMoved();
        if(settingsdial->exec()) {
            emit newPort(settingsdial->getBroadcastPort());
            emit newID(settingsdial->getBroadcastID());
        }
        settingsDialogOpen = false;
        cursorMoved();
    }
}


void FightclubTeleprinter::setPort(uint newport) {
    settingsdial->setPort(newport);
    emit newPort(newport);
}

void FightclubTeleprinter::setID(uint newid) {
    settingsdial->setID(newid);
    emit newID(newid);
}


uint FightclubTeleprinter::getBcastPort() { return bcastcli->getBcastPort(); }
uint FightclubTeleprinter::getBcastID()   { return bcastcli->getBcastSignature(); }


void FightclubTeleprinter::toggleRoomclock(bool showRClock) {
    if(roomclock && !showRClock)        refreshtimer->stop();
    else if (!roomclock && showRClock)  refreshtimer->start(30);

    roomclock = showRClock;
    ui->clockwidget->setRoomclock(showRClock);
}


void FightclubTeleprinter::updateElapsedTime(int elTime) {
    if (roomclock) return;
    QString displayedTime = timeToString(elTime);
    ui->lcdtimedisplay->display(displayedTime);
    ui->lcdtimedisplay->setDigitCount(displayedTime.length());
}


QString FightclubTeleprinter::timeToString(int time) {
    QTime temp = QTime(0,0,0,0);
    temp = temp.addMSecs(time);

    QString res;
    if(temp.hour() != 0) res = temp.toString("H:mm:ss");
    else                 res = temp.toString("mm:ss");

    return res;
}


void FightclubTeleprinter::updateTime() {
    QTime now = QTime::currentTime();
    QString displayNow = now.toString("HH:mm");
    if((now.second() % 2) != 0) displayNow[2] = ' ';

    ui->lcdtimedisplay->display(displayNow);
    ui->lcdtimedisplay->setDigitCount(displayNow.length());
}



void FightclubTeleprinter::setWindowFont(QString family) { this->setFont((family == nullptr)? defaultFont : QFont(family)); }

void FightclubTeleprinter::setFontScale(double newScale) {
    if(newScale > 0.5) fontScale = newScale;
    resizeEvent(new QResizeEvent(QSize(width(),height()),QSize(width(),height())));
}




void FightclubTeleprinter::enterFullscreenMode() {
    setWindowState(Qt::WindowFullScreen);
    cursorMoved();
}

void FightclubTeleprinter::enterNoConfigMode()   { settingsdial->enterNoConfigMode(); }




void FightclubTeleprinter::resizeEvent(QResizeEvent *event) {
    QFont phaselabelfont = ui->phaselabel->font();
    phaselabelfont.setPointSize((2 + height()*0.03)*fontScale);

    QFont infolabelfont = ui->perflabel->font();
    infolabelfont.setPointSize((4 + height()*0.015)*fontScale);

    ui->phaselabel->setFont(phaselabelfont);
    ui->problabel->setFont(infolabelfont);
    ui->perflabel->setFont(infolabelfont);

    QWidget::resizeEvent(event);
}


void FightclubTeleprinter::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_F1:
            openAboutDialog();
            break;

        case Qt::Key_S:
            if((QApplication::keyboardModifiers() & Qt::ControlModifier)
                    && (QApplication::keyboardModifiers() & Qt::ShiftModifier))
                openSettingsDialog();
            break;

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


void FightclubTeleprinter::mouseMoveEvent(QMouseEvent *event) {
    cursorMoved();
    QWidget::mouseMoveEvent(event);
}




void FightclubTeleprinter::cursorMoved() {
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    if((windowState() == Qt::WindowFullScreen) && !aboutDialogOpen && !settingsDialogOpen)
        hideCursorTimer->start();
    else hideCursorTimer->stop();
}

void FightclubTeleprinter::hideCursor() {
    if(!aboutDialogOpen && !settingsDialogOpen)
        QApplication::setOverrideCursor(Qt::BlankCursor);
}
