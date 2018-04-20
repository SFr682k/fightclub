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


#include "fightclubclient.h"
#include "ui_fightclubclient.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QModelIndex>
#include <QStandardPaths>


#include <QDebug>


FightclubClient::FightclubClient(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FightclubClient)
{
    ui->setupUi(this);

    previousPath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0);

    ui->peoplelabel->setText(QApplication::applicationName() + " " + QApplication::applicationVersion());

    ui->ppaadvindic->display("A");
    ui->ppcarryindic->display("C");
    ui->ppocarryindic->display("O");

    ui->cpaadvindic->display("A");
    ui->cpcarryindic->display("C");
    ui->cpocarryindic->display("O");

    ui->dummycdown->display("    ");
    ui->aadvcdown->display("    ");

    ui->elapsedtime->display(" --:-- ");
    ui->maxtime->display(" --:-- ");
    setPhaseProgress(0);

    ui->listofstages->setEnabled(false);
    ui->listofphases->setEnabled(false);

    ui->unloadStagesFile->setEnabled(false);
    ui->unloadPhasesFile->setEnabled(false);

    ui->dataSetDescr->setText(" ");
    ui->stagesFileDescr->setText(" ");
    ui->phasesFileDescr->setText(" ");

    ui->startstopbttn->setEnabled(false);
    ui->resettimebttn->setEnabled(false);
    ui->settimebttn->setEnabled(false);

    aboutdlg = new AboutDialog(this);

    bcastsrv = new BroadcastServer(this, QHostAddress("127.0.0.1"), 45454, 12345);
    ui->localhost->setChecked(true);
    ui->bcastportsel->setValue(45454);
    ui->bcastidsel->setValue(12345);

    lstadapt = new ListAdapter();
    phpbar = new PhasePBar();
    settimedlg = new SetTimeDialog(this);

    clockwindow = new ClockWindow();

    connect(ui->openClockWindowBttn, SIGNAL(clicked(bool)), this, SLOT(openClockWindow()));
    connect(clockwindow, SIGNAL(clockwindowClosed()), this, SLOT(clockWindowClosed()));


    connect(lstadapt, SIGNAL(forceInit()), this, SLOT(initialize()));

    connect(ui->actionAbout, SIGNAL(triggered()), aboutdlg, SLOT(exec()));




    // CONTROL TAB ----------------------------------------------------------------------

    connect(lstadapt, SIGNAL(phaseListModelChanged(QAbstractTableModel*)), this, SLOT(propagatePhasesList(QAbstractTableModel*)));
    connect(lstadapt, SIGNAL(stageListModelChanged(QAbstractTableModel*)), this, SLOT(propagateStagesList(QAbstractTableModel*)));

    connect(ui->phasebwd, SIGNAL(clicked()), lstadapt, SLOT(prevPhase()));
    connect(lstadapt, SIGNAL(enablePrevPhaseButton(bool)), ui->phasebwd, SLOT(setEnabled(bool)));
    connect(ui->phasefwd, SIGNAL(clicked()), lstadapt, SLOT(nextPhase()));
    connect(lstadapt, SIGNAL(enableNextPhaseButton(bool)), ui->phasefwd, SLOT(setEnabled(bool)));
    connect(lstadapt, SIGNAL(currentPhaseChanged(int)), this, SLOT(scrollToSelectedPhase(int)));
    connect(lstadapt, SIGNAL(currentStageIsRCS(bool)), ui->listofphases, SLOT(setDisabled(bool)));
    lstadapt->setUpPhaseSwitchingButtons();


    connect(lstadapt, SIGNAL(prevPhaseAAdv(bool)), this, SLOT(setPrevPhaseAAdv(bool)));
    connect(lstadapt, SIGNAL(prevPhaseCarry(bool)), this, SLOT(setPrevPhaseCarry(bool)));
    connect(lstadapt, SIGNAL(prevPhaseOCarry(bool)), this, SLOT(setPrevPhaseOCarry(bool)));
    connect(lstadapt, SIGNAL(currPhaseAAdv(bool)), this, SLOT(setCurrPhaseAAdv(bool)));
    connect(lstadapt, SIGNAL(currPhaseCarry(bool)), this, SLOT(setCurrPhaseCarry(bool)));
    connect(lstadapt, SIGNAL(currPhaseOCarry(bool)), this, SLOT(setCurrPhaseOCarry(bool)));


    connect(lstadapt, SIGNAL(resetTime()), phpbar, SLOT(resetTimer()));


    connect(ui->startstopbttn, SIGNAL(clicked()), phpbar, SLOT(startOrPause()));
    connect(ui->startstopbttn, SIGNAL(clicked()), this, SLOT(toggleStartStopBttn()));

    connect(ui->resettimebttn, SIGNAL(clicked()), phpbar, SLOT(resetTimer()));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), ui->resettimebttn, SLOT(setDisabled(bool)));

    connect(ui->settimebttn, SIGNAL(clicked()), this, SLOT(openSetTimeDialog()));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), ui->settimebttn, SLOT(setDisabled(bool)));
    connect(settimedlg, SIGNAL(elapsedTimeSet(int)), phpbar, SLOT(setElapsedTime(int)));
    connect(settimedlg, SIGNAL(remainingTimeSet(int)), phpbar, SLOT(setRemainingTime(int)));

    connect(lstadapt, SIGNAL(currentStageChanged(int)), this, SLOT(scrollToSelectedStage(int)));
    connect(lstadapt, SIGNAL(currentPhaseChanged(int)), this, SLOT(scrollToSelectedPhase(int)));


    connect(lstadapt, SIGNAL(phaseNameChanged(QString)), ui->phaselabel, SLOT(setText(QString)));
    connect(lstadapt, SIGNAL(phaseNameChanged(QString)), bcastsrv, SLOT(updatePhaseName(QString)));
    connect(lstadapt, SIGNAL(phaseNameChanged(QString)), clockwindow, SLOT(phaseNameChanged(QString)));

    connect(phpbar, SIGNAL(elapsedTimeUpdate(QString)), ui->elapsedtime, SLOT(display(QString)));
    connect(phpbar, SIGNAL(elapsedTimeUpdate(int)), bcastsrv, SLOT(updateElapsedTime(int)));
    connect(phpbar, SIGNAL(elapsedTimeUpdate(int)), clockwindow, SLOT(updateElapsedTime(int)));
    connect(lstadapt, SIGNAL(elapsedTimeChanged(int)), phpbar, SLOT(setElapsedTime(int)));

    connect(phpbar, SIGNAL(phaseProgressUpdate(double)), this, SLOT(setPhaseProgress(double)));

    connect(lstadapt, SIGNAL(maximumTimeChanged(int)), phpbar, SLOT(setMaximumTime(int)));
    connect(lstadapt, SIGNAL(maximumTimeChanged(int)), bcastsrv, SLOT(updateMaximumTime(int)));
    connect(lstadapt, SIGNAL(maximumTimeChanged(int)), clockwindow, SLOT(updateMaximumTime(int)));
    connect(phpbar, SIGNAL(maximumTimeUpdate(QString)), ui->maxtime, SLOT(display(QString)));

    connect(phpbar, SIGNAL(overtimed(int)), lstadapt, SLOT(handleOvertime(int)));
    connect(lstadapt, SIGNAL(overtimeCountdownChanged(QString)), ui->aadvcdown, SLOT(display(QString)));
    connect(lstadapt, SIGNAL(getElapsedOvertime()), phpbar, SLOT(getElapsedOvertime()));
    connect(lstadapt, SIGNAL(overtimeChanged(int)), phpbar, SLOT(setOvertime(int)));

    connect(lstadapt, SIGNAL(roomClockChanged(bool)), phpbar, SLOT(setRoomclock(bool)));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), bcastsrv, SLOT(updateRClockState(bool)));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), clockwindow, SLOT(toggleRoomclock(bool)));




    // BROADCAST TAB --------------------------------------------------------------------

    connect(ui->bcastipapply, SIGNAL(clicked(bool)), this, SLOT(setBroadcastIP()));
    connect(ui->bcastportapply, SIGNAL(clicked(bool)), this, SLOT(setBroadcastPort()));
    connect(ui->bcastidapply, SIGNAL(clicked(bool)), this, SLOT(setBroadcastID()));

    connect(ui->customippt1, SIGNAL(valueChanged(int)), this, SLOT(checkCustomIPRbttn()));
    connect(ui->customippt2, SIGNAL(valueChanged(int)), this, SLOT(checkCustomIPRbttn()));
    connect(ui->customippt3, SIGNAL(valueChanged(int)), this, SLOT(checkCustomIPRbttn()));
    connect(ui->customippt4, SIGNAL(valueChanged(int)), this, SLOT(checkCustomIPRbttn()));




    // FIGHTCLUB TAB --------------------------------------------------------------------

    connect(ui->loadStagesFile, SIGNAL(clicked(bool)), this, SLOT(openStagesFile()));
    connect(ui->unloadStagesFile, SIGNAL(clicked(bool)), this, SLOT(unloadStagesFile()));
    connect(ui->loadPhasesFile, SIGNAL(clicked(bool)), this, SLOT(openPhasesFile()));
    connect(ui->unloadPhasesFile, SIGNAL(clicked(bool)), this, SLOT(unloadPhasesFile()));
}


FightclubClient::~FightclubClient() {
    delete ui;
    delete aboutdlg;
}


void FightclubClient::initialize() {
    if(phpbar->isRunning()) {
        phpbar->startOrPause();
        toggleStartStopBttn();
    }

    phpbar->resetTimer();

    ui->peoplelabel->setText(QApplication::applicationName() + " " + QApplication::applicationVersion());

    ui->dummycdown->display("    ");
    ui->aadvcdown->display("    ");

    ui->elapsedtime->display(" --:-- ");
    ui->maxtime->display(" --:-- ");
    setPhaseProgress(0);

    ui->resettimebttn->setEnabled(false);
    ui->settimebttn->setEnabled(false);
}


bool FightclubClient::continueAndInit() {
    if((lstadapt->getCurrentStage() < 0) && (lstadapt->getCurrentPhase() < 0))
        /** The first stage/phase has not been entered. No progress to be lost. Continue! */
        return true;
    else {
        /** Progress could be lost. Ask the user whether he *really* wants to (un)load the file. */
        return QMessageBox::warning(this,
            "Continue?",
            "This action will reset the current progress.\nContinue?",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes;
    }
}


void FightclubClient::openClockWindow() {
    clockwindow->show();
    ui->openClockWindowBttn->setEnabled(false);
}

void FightclubClient::clockWindowClosed() { ui->openClockWindowBttn->setEnabled(true); }





// CONTROL TAB --------------------------------------------------------------------------

void FightclubClient::toggleStartStopBttn() {
    if(phpbar->isRunning()) {
        if(phpbar->isRoomclock() && ui->phasefwd->isEnabled()) lstadapt->nextPhase();
        else if((lstadapt->getCurrentStage() == -1) && (lstadapt->getCurrentPhase() == -1))
            lstadapt->nextPhase();

        ui->startstopbttn->setIcon(QIcon(":/breeze-icons/chronometer-pause-24.png"));
    } else ui->startstopbttn->setIcon(QIcon(":/breeze-icons/chronometer-start-24.png"));
}


void FightclubClient::openSetTimeDialog() {
    settimedlg->resetValues();
    settimedlg->setMaximumRTime(phpbar->getMaxTime());
    settimedlg->exec();
}


void FightclubClient::propagateStagesList(QAbstractTableModel* stagesList) {
    ui->listofstages->setModel(stagesList);
    ui->listofstages->setEnabled(stagesList->rowCount() > 0);
    ui->unloadStagesFile->setEnabled(stagesList->rowCount() > 0);

    if(stagesList->rowCount() == 0) {
        ui->stagesFileTitle->setText("No stages loaded");
        ui->stagesFileDescr->setText(" ");
    }
}

void FightclubClient::scrollToSelectedStage(int rownr) {
    if(rownr < 0) rownr = 0;
    QModelIndex id = ui->listofstages->model()->index(rownr, 0);
    ui->listofstages->scrollTo(id, QListView::PositionAtCenter);
}

void FightclubClient::propagatePhasesList(QAbstractTableModel* phasesList) {
    ui->listofphases->setModel(phasesList);
    ui->listofphases->setEnabled(phasesList->rowCount() > 0);
    ui->unloadPhasesFile->setEnabled(phasesList->rowCount() > 0);

    if(phasesList->rowCount() == 0) {
        ui->phasesFileTitle->setText("No phases loaded");
        ui->phasesFileDescr->setText(" ");
    }

    ui->startstopbttn->setEnabled(phasesList->rowCount() > 0);
}

void FightclubClient::scrollToSelectedPhase(int rownr) {
    if(rownr < 0) rownr = 0;
    QModelIndex id = ui->listofphases->model()->index(rownr, 0);
    ui->listofphases->scrollTo(id, QListView::PositionAtCenter);
}


void FightclubClient::setPrevPhaseAAdv(bool ppaadv) {
    if(ppaadv) ui->ppaadvindic->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    else       ui->ppaadvindic->setSegmentStyle(QLCDNumber::SegmentStyle::Outline);
}

void FightclubClient::setPrevPhaseCarry(bool ppcarry) {
    if(ppcarry) ui->ppcarryindic->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    else        ui->ppcarryindic->setSegmentStyle(QLCDNumber::SegmentStyle::Outline);
}

void FightclubClient::setPrevPhaseOCarry(bool ppocarry){
    if(ppocarry) ui->ppocarryindic->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    else         ui->ppocarryindic->setSegmentStyle(QLCDNumber::SegmentStyle::Outline);
}

void FightclubClient::setCurrPhaseAAdv(bool cpaadv) {
    if(cpaadv) ui->cpaadvindic->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    else       ui->cpaadvindic->setSegmentStyle(QLCDNumber::SegmentStyle::Outline);
}

void FightclubClient::setCurrPhaseCarry(bool cpcarry) {
    if(cpcarry) ui->cpcarryindic->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    else        ui->cpcarryindic->setSegmentStyle(QLCDNumber::SegmentStyle::Outline);
}

void FightclubClient::setCurrPhaseOCarry(bool cpocarry) {
    if(cpocarry) ui->cpocarryindic->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    else         ui->cpocarryindic->setSegmentStyle(QLCDNumber::SegmentStyle::Outline);
}



void FightclubClient::setPhaseProgress(double progress) {
    ui->progressbar->setMaximum(1000);

    QString lowerbborder;
    QString upperbborder;

    if(((int) (progress*1000.0) % 1000) == 0) {
        lowerbborder = "0";
        upperbborder = "0.00001";
    } else {
        lowerbborder = QString::number(((int) (progress*1000.0) % 1000)/1000.0 - 0.00001);
        upperbborder = QString::number(((int) (progress*1000.0) % 1000)/1000.0);
    }

    if(progress < 0.75)      ui->progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: #32c81e;}");
    else if(progress <= 1.0) ui->progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: #ffbe1e;}");
    else if(progress <= 2.0) ui->progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: QLinearGradient(x1: 0,y1: 0,x2: 1, y2: 0, stop: 0 #c80000, stop: " + lowerbborder + " #c80000, stop: " + upperbborder + " #ffbe1e, stop: 1 #ffbe1e);}");
    else if(progress <= 3.0) ui->progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: QLinearGradient(x1: 0,y1: 0,x2: 1, y2: 0, stop: 0 #641e64, stop: " + lowerbborder + " #641e64, stop: " + upperbborder + " #c80000, stop: 1 #c80000);}");
    else if(progress > 3.0)  ui->progressbar->setStyleSheet("QProgressBar::chunk{ border-radius: 3px; background-color: #641e64;}");

    if(progress <= 1.0) ui->progressbar->setValue(((int) (progress*1000)) % 1000);
    else                ui->progressbar->setValue(1000);
}







// BROADCAST TAB ------------------------------------------------------------------------

void FightclubClient::setBroadcastIP() {
    QString bcastIP;
    if(ui->localhost->isChecked())      bcastIP = "127.0.0.1";
    else if(ui->everybody->isChecked()) bcastIP = "255.255.255.255";
    else if(ui->customip->isChecked()) {
        bcastIP = QString::number(ui->customippt1->value()) + "."
                  + QString::number(ui->customippt2->value()) + "."
                  + QString::number(ui->customippt3->value()) + "."
                  + QString::number(ui->customippt4->value());
    }
    bcastsrv->setBroadcastAddress(bcastIP);
}

void FightclubClient::setBroadcastPort() { bcastsrv->setBroadcastPort(ui->bcastportsel->value()); }
void FightclubClient::setBroadcastID()   { bcastsrv->setSignature(ui->bcastidsel->value()); }

void FightclubClient::checkCustomIPRbttn() { ui->customip->setChecked(true); }







// FIGHTCLUB TAB ------------------------------------------------------------------------

void FightclubClient::openStagesFile() {
    if(continueAndInit()) {
        QFileDialog *selectStagesFileDialog
                = new QFileDialog(this, "Select a stages file", previousPath, "Fightclub stages files (*.fcstages)");

        if(selectStagesFileDialog->exec()) {
            lstadapt->loadStagesListFromFile(selectStagesFileDialog->selectedFiles().value(0));
            previousPath = selectStagesFileDialog->directory().absolutePath();
            ui->stagesFileTitle->setText(selectStagesFileDialog->selectedFiles().value(0));
        }
    }
}


void FightclubClient::openPhasesFile() {
    if(continueAndInit()) {
        QFileDialog *selectPhasesFileDialog
                = new QFileDialog(this, "Select a phases file", previousPath, "Fightclub phases files (*.fcphases)");

        if(selectPhasesFileDialog->exec()) {
            lstadapt->loadPhasesListFromFile(selectPhasesFileDialog->selectedFiles().value(0));
            previousPath = selectPhasesFileDialog->directory().absolutePath();
            ui->phasesFileTitle->setText(selectPhasesFileDialog->selectedFiles().value(0));
        }
    }
}


void FightclubClient::unloadStagesFile() { if(continueAndInit()) lstadapt->unloadStagesList(); }
void FightclubClient::unloadPhasesFile() { if(continueAndInit()) lstadapt->unloadPhasesList(); }
