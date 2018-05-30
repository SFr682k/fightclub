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


#include "fightclubnano.h"
#include "ui_fightclubnano.h"

#include "aboutdialog.h"

#include <QTime>

FightclubNano::FightclubNano(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FightclubNano)
{
    ui->setupUi(this);

    ui->controlToolbar->setContextMenuPolicy(Qt::PreventContextMenu);

    ui->actionActivateShortcuts->setChecked(true);
    enableShortcuts = true;

    if(ui->phaselabel->text().startsWith("<ApplicationName>"))
            ui->phaselabel->setText(QApplication::applicationName().append(", Version ").append(QApplication::applicationVersion()));


    ui->actionMinus10->setEnabled(false);
    ui->actionPlus10->setEnabled(false);
    ui->actionSetTime->setEnabled(false);
    ui->actionReset->setEnabled(false);


    roomclock = true;

    defaultFont = font();
    fontScale = 1.0, buttonScale = 1.0;


    clklgk = new ClockLogic();
    lstadapt = new ListAdapter();
    settimedlg = new SetTimeDialog(this);
    settingsdlg = new SettingsDialog(this);


    refreshtimer = new QTimer();


    connect(ui->actionBwd, SIGNAL(triggered(bool)), this, SLOT(bwdEvent()));
    connect(ui->actionFwd, SIGNAL(triggered(bool)), this, SLOT(fwdEvent()));
    connect(ui->actionStartPause, SIGNAL(triggered(bool)), this, SLOT(startstopEvent()));
    connect(ui->actionReset, SIGNAL(triggered(bool)), this, SLOT(resetTimeEvent()));
    connect(ui->actionMinus10, SIGNAL(triggered(bool)), this, SLOT(minusTenEvent()));
    connect(ui->actionPlus10, SIGNAL(triggered(bool)), this, SLOT(plusTenEvent()));
    connect(ui->actionSetTime, SIGNAL(triggered(bool)), this, SLOT(setTimeEvent()));

    connect(ui->actionActivateShortcuts, SIGNAL(toggled(bool)), this, SLOT(activateShortcuts(bool)));


    connect(clklgk, SIGNAL(elapsedTimeUpdate(int)), ui->clockwidget, SLOT(setElapsedTime(int)));
    connect(refreshtimer, SIGNAL(timeout()), ui->clockwidget, SLOT(act()));

    connect(clklgk, SIGNAL(elapsedTimeUpdate(QString)), ui->lcdtimedisplay, SLOT(display(QString)));
    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateLCDDisplay()));




    connect(lstadapt, SIGNAL(enablePrevPhaseButton(bool)), ui->actionBwd, SLOT(setEnabled(bool)));
    connect(lstadapt, SIGNAL(enableNextPhaseButton(bool)), ui->actionFwd, SLOT(setEnabled(bool)));

    connect(lstadapt, SIGNAL(carryElapsedOvertime()), clklgk, SLOT(carryOvertime()));
    connect(lstadapt, SIGNAL(resetTime()), clklgk, SLOT(resetTime()));

    connect(lstadapt, SIGNAL(maximumTimeChanged(int)), clklgk, SLOT(setMaximumTime(int)));
    connect(lstadapt, SIGNAL(maximumTimeChanged(int)), ui->clockwidget, SLOT(setMaximumTime(int)));

    connect(lstadapt, SIGNAL(overtimeChanged(int)), clklgk, SLOT(setMaximumOvertime(int)));
    connect(clklgk, SIGNAL(overtimed(int)), lstadapt, SLOT(handleOvertime(int)));

    connect(lstadapt, SIGNAL(phaseLabelChanged(QString)), ui->phaselabel, SLOT(setText(QString)));

    connect(lstadapt, SIGNAL(roomClockChanged(bool)), ui->clockwidget, SLOT(setRoomclock(bool)));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), clklgk, SLOT(setRoomclock(bool)));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), this, SLOT(setRoomclock(bool)));

    connect(lstadapt, SIGNAL(forceInit()), this, SLOT(init()));


    connect(settimedlg, SIGNAL(elapsedTimeSet(int)), clklgk, SLOT(setElapsedTime(int)));
    connect(settimedlg, SIGNAL(remainingTimeSet(int)), clklgk, SLOT(setRemainingTime(int)));


    connect(settingsdlg, SIGNAL(loadListOfPhases(QString)), lstadapt, SLOT(loadPhasesListFromFile(QString)));
    connect(settingsdlg, SIGNAL(loadListOfNanoPhases(QString)), lstadapt, SLOT(loadNanoPhasesListFromFile(QString)));
    connect(settingsdlg, SIGNAL(unloadListOfPhases()), lstadapt, SLOT(unloadPhasesList()));

    connect(settingsdlg, SIGNAL(fontChanged(QString)), this, SLOT(setApplicationFont(QString)));
    connect(settingsdlg, SIGNAL(fontScaleChanged(double)), this, SLOT(setFontScale(double)));
    connect(settingsdlg, SIGNAL(buttonScaleChanged(double)), this, SLOT(setButtonScale(double)));


    lstadapt->setUpPhaseSwitchingButtons();
    refreshtimer->start(30);

    aboutDialogOpen = false, settingsDialogOpen = false;
}


FightclubNano::~FightclubNano() {
    delete ui;
}



void FightclubNano::bwdEvent()       { lstadapt->bwd(); }
void FightclubNano::fwdEvent()       { lstadapt->fwd(); }

void FightclubNano::startstopEvent() {
    clklgk->startOrPause();
    this->toggleStartStopBttn();
}

void FightclubNano::resetTimeEvent() { clklgk->resetTime(); }
void FightclubNano::minusTenEvent()  { clklgk->minusTen(); }
void FightclubNano::plusTenEvent()   { clklgk->plusTen(); }
void FightclubNano::setTimeEvent()   { this->openSetTimeDialog(); }





void FightclubNano::openAboutDialog() {
    if(!aboutDialogOpen) {
        aboutDialogOpen = true;
        AboutDialog *ad = new AboutDialog(this);
        ad->exec();
        aboutDialogOpen = false;
    }
}

void FightclubNano::openSettingsDialog() {
    if(!settingsDialogOpen) {
        settingsDialogOpen = true;
        settingsdlg->exec();
        settingsDialogOpen = false;
    }
}



void FightclubNano::init() {
    if(clklgk->isRunning()){
        clklgk->startOrPause();
        toggleStartStopBttn();
    }

    clklgk->resetTime();
}




void FightclubNano::toggleStartStopBttn() {
    if(clklgk->isRunning()) {
        if(clklgk->isRoomclock() && ui->actionFwd->isEnabled()) lstadapt->nextPhase();
        else if(lstadapt->getCurrentPhase() == -1) lstadapt->nextPhase();

        ui->actionStartPause->setIcon(QIcon(":/breeze-icons/chronometer-pause-24.svg"));
    } else ui->actionStartPause->setIcon(QIcon(":/breeze-icons/chronometer-start-24.svg"));

    ui->actionStartPause->setText(clklgk->isRunning()? "Pause" : "Start");
    ui->actionStartPause->setToolTip(clklgk->isRunning()? "Pause" : "Start");
}

void FightclubNano::openSetTimeDialog() {
    if(roomclock) return;

    settimedlg->resetValues();
    settimedlg->setMaximumRTime(clklgk->getMaxTime());
    settimedlg->setElapsedTime(clklgk->getElapsedTime());
    settimedlg->exec();
}




void FightclubNano::updateLCDDisplay() {
    if(!roomclock) return;

    QTime now = QTime::currentTime();
    QString displayNow = now.toString(" HH:mm ");
    if((now.second() % 2) != 0) displayNow[3] = ' ';

    ui->lcdtimedisplay->display(displayNow);
}


void FightclubNano::setRoomclock(bool rclk) {
    roomclock = rclk;
    ui->actionReset->setDisabled(roomclock);
    ui->actionMinus10->setDisabled(roomclock);
    ui->actionSetTime->setDisabled(roomclock);
    ui->actionPlus10->setDisabled(roomclock);
}





void FightclubNano::activateShortcuts(bool activate) { enableShortcuts = activate; }





void FightclubNano::setApplicationFont(QString font)
    { this->setFont((font == nullptr)? defaultFont : QFont(font)); }

void FightclubNano::setFontScale(double newScale) {
    if(newScale > 0.5) fontScale = newScale;
    resizeEvent(new QResizeEvent(QSize(width(),height()),QSize(width(),height())));
}

void FightclubNano::setButtonScale(double newScale) {
    if(newScale > 0.5) buttonScale = newScale;
    resizeEvent(new QResizeEvent(QSize(width(),height()),QSize(width(),height())));
}




void FightclubNano::resizeEvent(QResizeEvent *event) {
    QFont phaselabelfont = ui->phaselabel->font();
    phaselabelfont.setPointSize((2 + height()*0.03)*fontScale);
    ui->phaselabel->setFont(phaselabelfont);

    ui->controlToolbar->setIconSize(QSize(height()*0.041*buttonScale, height()*0.041*buttonScale));

    QWidget::resizeEvent(event);
}


void FightclubNano::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_Space:
            if(enableShortcuts) startstopEvent();
            break;


        case Qt::Key_B:
            if(enableShortcuts) bwdEvent();
            break;

        case Qt::Key_F:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                setWindowState(Qt::WindowFullScreen);
            else if(enableShortcuts) fwdEvent();
            break;


        case Qt::Key_R:
            if(enableShortcuts) resetTimeEvent();
            break;


        case Qt::Key_S:
            if((QApplication::keyboardModifiers() & Qt::ControlModifier)
                    && (QApplication::keyboardModifiers() & Qt::ShiftModifier))
                openSettingsDialog();
            else if(enableShortcuts) setTimeEvent();
            break;

        case Qt::Key_Minus:
            if(enableShortcuts) minusTenEvent();
            break;

        case Qt::Key_Plus:
            if(enableShortcuts) plusTenEvent();
            break;


        case Qt::Key_F1:
            openAboutDialog();
            break;

        case Qt::Key_Escape:
            if(windowState() == Qt::WindowFullScreen)
                setWindowState(Qt::WindowMaximized);
            break;

        case Qt::Key_Q:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                this->close();
            break;

        default:
            QWidget::keyPressEvent(event);
    }
}
