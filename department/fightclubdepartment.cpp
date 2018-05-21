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


#include "fightclubdepartment.h"
#include "ui_fightclubdepartment.h"

#include "clockwindow.h"
#include "filepropertyparser.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QModelIndex>
#include <QStandardPaths>


#include <QDebug>


FightclubDepartment::FightclubDepartment(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FightclubDepartment)
{
    ui->setupUi(this);


    exitEnabled = true, toggleFscreenEnabled = true;


    previousPath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0);

    ui->performerslabel->setText(QApplication::applicationName() + " " + QApplication::applicationVersion());

    ui->ppaadvindic->display("A");
    ui->ppcarryindic->display("C");
    ui->ppocarryindic->display("O");

    ui->cpaadvindic->display("A");
    ui->cpcarryindic->display("C");
    ui->cpocarryindic->display("O");

    ui->aadvcdown->display("    ");

    ui->elapsedtime->display(" --:-- ");
    ui->maxtime->display(" --:-- ");
    setPhaseProgress(0);

    ui->listofstages->setEnabled(false);
    ui->listofphases->setEnabled(false);
    ui->problemcombobox->setEnabled(false);
    ui->problemcombobox->view()->setTextElideMode(Qt::ElideRight);
    ui->toggleEditProblemBttn->setEnabled(false);

    ui->repcombobox->setEnabled(false);
    ui->toggleEditRepBttn->setEnabled(false);
    ui->repcombobox->view()->setTextElideMode(Qt::ElideRight);
    ui->oppcombobox->setEnabled(false);
    ui->toggleEditOppBttn->setEnabled(false);
    ui->oppcombobox->view()->setTextElideMode(Qt::ElideRight);
    ui->revcombobox->setEnabled(false);
    ui->toggleEditRevBttn->setEnabled(false);
    ui->revcombobox->view()->setTextElideMode(Qt::ElideRight);


    ui->unloadStagesFile->setEnabled(false);
    ui->unloadPhasesFile->setEnabled(false);
    ui->unloadProblemsFile->setEnabled(false);
    ui->unloadTeamsFile->setEnabled(false);

    ui->dataSetDescr->setText(" ");
    ui->stagesFileDescr->setText(" ");
    ui->phasesFileDescr->setText(" ");
    ui->problemsFileDescr->setText(" ");
    ui->teamsFileDescr->setText(" ");

    ui->startstopbttn->setEnabled(false);
    ui->resettimebttn->setEnabled(false);
    ui->savetimebttn->setEnabled(false);
    ui->settimebttn->setEnabled(false);
    ui->timePlus10bttn->setEnabled(false);
    ui->timeMinus10bttn->setEnabled(false);



    aboutdlg = new AboutDialog(this);

    bcastsrv = new BroadcastServer(this, QHostAddress("127.0.0.1"), 45454, 12345);
    ui->bcastActivated->setChecked(false);
    ui->listOfBroadcasts->setEnabled(false);
    ui->bcastConfigBox->setEnabled(false);

    ui->selectIPCombobox->addItem("Local machine", QVariant(IP_LOCAL));
    ui->selectIPCombobox->addItem("Local network", QVariant(IP_BCAST));
    ui->selectIPCombobox->addItem("Custom machine", QVariant(IP_CUSTOM));
    updateBcastIPBoxes();

    // disable not implemented stuff
    ui->lockBcastTabBttn->setEnabled(false);
    ui->addBroadcast->setEnabled(false);
    ui->deleteBroadcast->setEnabled(false);

    ui->bcastportsel->setValue(45454);
    ui->bcastidsel->setValue(12345);

    lstadapt = new ListAdapter();
    phpbar = new PhasePBar();
    probadapt = new ProblemAdapter();

    settimedlg = new SetTimeDialog(this);
    connect(phpbar, SIGNAL(lastAutosavedTimeUpdate(int)), settimedlg, SLOT(updateAutosavedTime(int)));

    teamadapt = new TeamAdapter();
    lstadapt->setTeamAdapter(teamadapt);


    ui->actionFullscreen->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_F));
    connect(ui->actionFullscreen, SIGNAL(triggered(bool)), this, SLOT(activateFullscreen(bool)));
    connect(ui->actionAbout, SIGNAL(triggered()), aboutdlg, SLOT(exec()));


    connect(ui->actionOpen_Clock_Window, SIGNAL(triggered(bool)), this, SLOT(openClockWindow()));

    clkwindowsettings = new ClkWindowSettings(this);
    connect(ui->actionClkWindowSettings, SIGNAL(triggered(bool)), clkwindowsettings, SLOT(exec()));


    connect(lstadapt, SIGNAL(forceInit()), this, SLOT(initialize()));






    // CONTROL TAB ----------------------------------------------------------------------

    connect(lstadapt, SIGNAL(phaseListModelChanged(QAbstractTableModel*)), this, SLOT(propagatePhasesList(QAbstractTableModel*)));
    connect(lstadapt, SIGNAL(stageListModelChanged(QAbstractTableModel*)), this, SLOT(propagateStagesList(QAbstractTableModel*)));

    connect(ui->phasebwd, SIGNAL(clicked()), lstadapt, SLOT(bwd()));
    connect(lstadapt, SIGNAL(enablePrevPhaseButton(bool)), ui->phasebwd, SLOT(setEnabled(bool)));

    connect(ui->phasefwd, SIGNAL(clicked()), lstadapt, SLOT(fwd()));
    connect(lstadapt, SIGNAL(enableNextPhaseButton(bool)), ui->phasefwd, SLOT(setEnabled(bool)));

    connect(lstadapt, SIGNAL(currentStageChanged(int)), this, SLOT(scrollToSelectedStage(int)));
    connect(lstadapt, SIGNAL(currentPhaseChanged(int)), this, SLOT(scrollToSelectedPhase(int)));
    connect(lstadapt, SIGNAL(currentStageIsRCS(bool)), ui->listofphases, SLOT(setDisabled(bool)));


    connect(this, SIGNAL(switchStages(bool)), this, SLOT(switchBetweenStages(bool)));
    connect(this, SIGNAL(switchStages(bool)), lstadapt, SLOT(switchStages(bool)));

    lstadapt->setUpPhaseSwitchingButtons();



    connect(lstadapt, SIGNAL(prevPhasePropsChanged(bool,bool,bool)), this, SLOT(setPrevPhaseProps(bool,bool,bool)));
    connect(lstadapt, SIGNAL(currPhasePropsChanged(bool,bool,bool)), this, SLOT(setCurrPhaseProps(bool,bool,bool)));


    connect(lstadapt, SIGNAL(resetTime()), phpbar, SLOT(resetTimer()));


    connect(ui->startstopbttn, SIGNAL(clicked()), phpbar, SLOT(startOrPause()));
    connect(ui->startstopbttn, SIGNAL(clicked()), this, SLOT(toggleStartStopBttn()));

    connect(ui->resettimebttn, SIGNAL(clicked()), phpbar, SLOT(resetTimer()));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), ui->resettimebttn, SLOT(setDisabled(bool)));

    connect(ui->savetimebttn, SIGNAL(clicked(bool)), phpbar, SLOT(saveCurrentTime()));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), ui->savetimebttn, SLOT(setDisabled(bool)));

    connect(ui->settimebttn, SIGNAL(clicked()), this, SLOT(openSetTimeDialog()));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), ui->settimebttn, SLOT(setDisabled(bool)));
    connect(settimedlg, SIGNAL(elapsedTimeSet(int)), phpbar, SLOT(setElapsedTime(int)));
    connect(settimedlg, SIGNAL(remainingTimeSet(int)), phpbar, SLOT(setRemainingTime(int)));

    connect(ui->timeMinus10bttn, SIGNAL(clicked(bool)), phpbar, SLOT(timeMinus10()));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), ui->timeMinus10bttn, SLOT(setDisabled(bool)));

    connect(ui->timePlus10bttn, SIGNAL(clicked(bool)), phpbar, SLOT(timePlus10()));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), ui->timePlus10bttn, SLOT(setDisabled(bool)));


    connect(lstadapt, SIGNAL(phaseNameChanged(QString)), ui->phaselabel, SLOT(setText(QString)));
    connect(lstadapt, SIGNAL(phaseNameChanged(QString)), bcastsrv, SLOT(updatePhaseName(QString)));

    connect(phpbar, SIGNAL(elapsedTimeUpdate(QString)), ui->elapsedtime, SLOT(display(QString)));
    connect(phpbar, SIGNAL(elapsedTimeUpdate(int)), bcastsrv, SLOT(updateElapsedTime(int)));
    connect(lstadapt, SIGNAL(elapsedTimeChanged(int)), phpbar, SLOT(setElapsedTime(int)));

    connect(phpbar, SIGNAL(phaseProgressUpdate(double)), this, SLOT(setPhaseProgress(double)));

    connect(lstadapt, SIGNAL(maximumTimeChanged(int)), phpbar, SLOT(setMaximumTime(int)));
    connect(lstadapt, SIGNAL(maximumTimeChanged(int)), bcastsrv, SLOT(updateMaximumTime(int)));
    connect(phpbar, SIGNAL(maximumTimeUpdate(QString)), ui->maxtime, SLOT(display(QString)));

    connect(phpbar, SIGNAL(overtimed(int)), lstadapt, SLOT(handleOvertime(int)));
    connect(lstadapt, SIGNAL(overtimeCountdownChanged(QString)), ui->aadvcdown, SLOT(display(QString)));
    connect(lstadapt, SIGNAL(getElapsedOvertime()), phpbar, SLOT(getElapsedOvertime()));
    connect(lstadapt, SIGNAL(overtimeChanged(int)), phpbar, SLOT(setOvertime(int)));

    connect(lstadapt, SIGNAL(roomClockChanged(bool)), phpbar, SLOT(setRoomclock(bool)));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), bcastsrv, SLOT(updateRClockState(bool)));

    connect(lstadapt, SIGNAL(currentProblemChanged(int)), this, SLOT(propagateProblemsList(int)));
    connect(ui->problemcombobox, SIGNAL(currentIndexChanged(QString)), bcastsrv, SLOT(updateProblem(QString)));
    connect(ui->toggleEditProblemBttn, SIGNAL(clicked(bool)), this, SLOT(editProblemBttnToggled()));

    connect(lstadapt, SIGNAL(currentPerformersChanged(QString,QString,QString)), this, SLOT(performersChanged(QString,QString,QString)));

    connect(ui->repcombobox, SIGNAL(activated(int)), this, SLOT(updateReporterModel(int)));
    connect(ui->repcombobox, SIGNAL(activated(QString)), lstadapt, SLOT(reporterChanged(QString)));
    connect(ui->toggleEditRepBttn, SIGNAL(clicked(bool)), this, SLOT(editRepBttnToggled()));

    connect(ui->oppcombobox, SIGNAL(activated(int)), this, SLOT(updateOpponentModel(int)));
    connect(ui->oppcombobox, SIGNAL(activated(QString)), lstadapt, SLOT(opponentChanged(QString)));
    connect(ui->toggleEditOppBttn, SIGNAL(clicked(bool)), this, SLOT(editOppBttnToggled()));

    connect(ui->revcombobox, SIGNAL(activated(int)), this, SLOT(updateReviewerModel(int)));
    connect(ui->revcombobox, SIGNAL(activated(QString)), lstadapt, SLOT(reviewerChanged(QString)));
    connect(ui->toggleEditRevBttn, SIGNAL(clicked(bool)), this, SLOT(editRevBttnToggled()));

    connect(lstadapt, SIGNAL(performersChanged(QString)), ui->performerslabel, SLOT(setText(QString)));
    connect(lstadapt, SIGNAL(performersChanged(QString)), bcastsrv, SLOT(updatePerformers(QString)));



    // BROADCAST TAB --------------------------------------------------------------------

    connect(ui->bcastActivated, SIGNAL(clicked(bool)), bcastsrv, SLOT(enableBroadcast(bool)));
    connect(ui->bcastActivated, SIGNAL(clicked(bool)), ui->listOfBroadcasts, SLOT(setEnabled(bool)));
    connect(ui->bcastActivated, SIGNAL(clicked(bool)), ui->bcastConfigBox, SLOT(setEnabled(bool)));

    connect(ui->selectIPCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBcastIPBoxes()));
    connect(ui->applyBcastSettings, SIGNAL(clicked(bool)), this, SLOT(applyBcastSettings()));



    // FIGHTCLUB TAB --------------------------------------------------------------------

    connect(ui->loadStagesFile, SIGNAL(clicked(bool)), this, SLOT(openStagesFile()));
    connect(ui->unloadStagesFile, SIGNAL(clicked(bool)), this, SLOT(unloadStagesFile()));
    connect(ui->loadPhasesFile, SIGNAL(clicked(bool)), this, SLOT(openPhasesFile()));
    connect(ui->unloadPhasesFile, SIGNAL(clicked(bool)), this, SLOT(unloadPhasesFile()));
    connect(ui->loadProblemsFile, SIGNAL(clicked(bool)), this, SLOT(openProblemsFile()));
    connect(ui->unloadProblemsFile, SIGNAL(clicked(bool)), this, SLOT(unloadProblemsFile()));
    connect(ui->loadTeamsFile, SIGNAL(clicked(bool)), this, SLOT(openTeamsFile()));
    connect(ui->unloadTeamsFile, SIGNAL(clicked(bool)), this, SLOT(unloadTeamsFile()));
}


FightclubDepartment::~FightclubDepartment() {
    delete ui;
    delete aboutdlg;
    delete settimedlg;
}


void FightclubDepartment::initialize() {
    if(phpbar->isRunning()) {
        phpbar->startOrPause();
        toggleStartStopBttn();
    }

    phpbar->resetTimer();

    ui->aadvcdown->display("    ");

    ui->elapsedtime->display(" --:-- ");
    ui->maxtime->display(" --:-- ");
    setPhaseProgress(0);

    ui->resettimebttn->setEnabled(false);
    ui->settimebttn->setEnabled(false);
}


bool FightclubDepartment::continueAndInit() {
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


void FightclubDepartment::openClockWindow() {
    ClockWindow *clkwindow = new ClockWindow();

    connect(lstadapt, SIGNAL(phaseNameChanged(QString)), clkwindow, SLOT(phaseNameChanged(QString)));
    connect(phpbar, SIGNAL(elapsedTimeUpdate(int)), clkwindow, SLOT(updateElapsedTime(int)));
    connect(lstadapt, SIGNAL(maximumTimeChanged(int)), clkwindow, SLOT(updateMaximumTime(int)));
    connect(lstadapt, SIGNAL(roomClockChanged(bool)), clkwindow, SLOT(toggleRoomclock(bool)));
    connect(ui->problemcombobox, SIGNAL(currentIndexChanged(QString)), clkwindow, SLOT(problemChanged(QString)));
    connect(lstadapt, SIGNAL(performersChanged(QString)), clkwindow, SLOT(performersChanged(QString)));

    connect(clkwindowsettings, SIGNAL(fontChanged(QString)), clkwindow, SLOT(setWindowFont(QString)));
    connect(clkwindowsettings, SIGNAL(fontScaleChanged(double)), clkwindow, SLOT(setFontScale(double)));
    connect(clkwindowsettings, SIGNAL(showRclockSecondHand(bool)), clkwindow, SLOT(showRclockSecondHand(bool)));
    connect(clkwindowsettings, SIGNAL(rclockBehaviorChanged(int)), clkwindow, SLOT(setRclockBehavior(int)));

    clkwindow->updateElapsedTime(phpbar->getElapsedTime());
    clkwindow->updateMaximumTime(phpbar->getMaxTime());
    clkwindow->toggleRoomclock(phpbar->isRoomclock());

    // TODO: Find a better solution for obtaining the current phase name?
    clkwindow->phaseNameChanged(ui->phaselabel->text());
    clkwindow->problemChanged(ui->problemcombobox->currentText());
    clkwindow->performersChanged(ui->performerslabel->text());

    clkwindow->setWindowFont(clkwindowsettings->getFontFamily());
    clkwindow->setFontScale(clkwindowsettings->getFontScale());
    clkwindow->showRclockSecondHand(clkwindowsettings->getShowRClkSecondHand());
    clkwindow->setRclockBehavior(clkwindowsettings->getRClkBehavior());

    connect(ui->actionClose_Clock_Windows, SIGNAL(triggered(bool)), clkwindow, SLOT(close()));
    connect(this, SIGNAL(closeAllClockWindows()), clkwindow, SLOT(close()));

    clkwindow->show();
}







// CONTROL TAB --------------------------------------------------------------------------

void FightclubDepartment::switchBetweenStages(bool swStages) {
    ui->phasebwd->setIcon(QIcon(QString(":/breeze-icons/go-").append(swStages? "first" : "previous").append("-32.svg")));
    ui->phasefwd->setIcon(QIcon(QString(":/breeze-icons/go-").append(swStages? "last"  : "next"    ).append("-32.svg")));
}

void FightclubDepartment::toggleStartStopBttn() {
    if(phpbar->isRunning()) {
        if(phpbar->isRoomclock() && ui->phasefwd->isEnabled()) lstadapt->nextPhase();
        else if((lstadapt->getCurrentStage() == -1) && (lstadapt->getCurrentPhase() == -1))
            lstadapt->nextPhase();

        ui->startstopbttn->setIcon(QIcon(":/breeze-icons/chronometer-pause-24.svg"));
    } else ui->startstopbttn->setIcon(QIcon(":/breeze-icons/chronometer-start-24.svg"));
}


void FightclubDepartment::openSetTimeDialog() {
    settimedlg->resetValues();
    settimedlg->setMaximumRTime(phpbar->getMaxTime());
    settimedlg->setSavedTime(phpbar->getLastSavedTime());
    settimedlg->setElapsedTime(phpbar->getElapsedTime());
    settimedlg->exec();
}


void FightclubDepartment::propagateStagesList(QAbstractTableModel* stagesList) {
    ui->listofstages->setModel(stagesList);
    ui->listofstages->setEnabled(stagesList->rowCount() > 0);
    ui->unloadStagesFile->setEnabled(stagesList->rowCount() > 0);

    if(stagesList->rowCount() == 0) {
        ui->stagesFileTitle->setText("No stages loaded");
        ui->stagesFileDescr->setText(" ");
    }
}

void FightclubDepartment::scrollToSelectedStage(int rownr) {
    if(rownr < 0) rownr = 0;
    QModelIndex id = ui->listofstages->model()->index(rownr, 0);
    ui->listofstages->scrollTo(id, QListView::PositionAtCenter);
}

void FightclubDepartment::propagatePhasesList(QAbstractTableModel* phasesList) {
    ui->listofphases->setModel(phasesList);
    ui->listofphases->setEnabled(phasesList->rowCount() > 0);
    ui->unloadPhasesFile->setEnabled(phasesList->rowCount() > 0);

    if(phasesList->rowCount() == 0) {
        ui->phasesFileTitle->setText("No phases loaded");
        ui->phasesFileDescr->setText(" ");
    }

    ui->startstopbttn->setEnabled(phasesList->rowCount() > 0);
}

void FightclubDepartment::scrollToSelectedPhase(int rownr) {
    if(rownr < 0) rownr = 0;
    QModelIndex id = ui->listofphases->model()->index(rownr, 0);
    ui->listofphases->scrollTo(id, QListView::PositionAtCenter);
}



void FightclubDepartment::setPrevPhaseProps(bool aadv, bool carry, bool ocarry) {
    ui->ppaadvindic->setSegmentStyle(aadv? QLCDNumber::SegmentStyle::Flat : QLCDNumber::SegmentStyle::Outline);
    ui->ppcarryindic->setSegmentStyle(carry? QLCDNumber::SegmentStyle::Flat : QLCDNumber::SegmentStyle::Outline);
    ui->ppocarryindic->setSegmentStyle(ocarry? QLCDNumber::SegmentStyle::Flat : QLCDNumber::SegmentStyle::Outline);
}

void FightclubDepartment::setCurrPhaseProps(bool aadv, bool carry, bool ocarry) {
    ui->cpaadvindic->setSegmentStyle(aadv? QLCDNumber::SegmentStyle::Flat : QLCDNumber::SegmentStyle::Outline);
    ui->cpcarryindic->setSegmentStyle(carry? QLCDNumber::SegmentStyle::Flat : QLCDNumber::SegmentStyle::Outline);
    ui->cpocarryindic->setSegmentStyle(ocarry? QLCDNumber::SegmentStyle::Flat : QLCDNumber::SegmentStyle::Outline);
}



void FightclubDepartment::setPhaseProgress(double progress) {
    ui->progressbar->setMaximum(2000);

    QString lowerbborder, upperbborder;
    QString leftColor, rightColor;

    if(((int) (progress*2000.0) % 2000) == 0) {
        lowerbborder = "0";
        upperbborder = "0.00001";
    } else {
        lowerbborder = QString::number(((int) (progress*2000.0) % 2000)/2000.0 - 0.00001);
        upperbborder = QString::number(((int) (progress*2000.0) % 2000)/2000.0);
    }

    if(progress < 0.75)      leftColor = "#32c81e", rightColor = "#fff";
    else if(progress <  1.0) leftColor = "#ffbe1e", rightColor = "#fff";
    else if(progress <  2.0) leftColor = "#c80000", rightColor = "#ffbe1e";
    else if(progress <  3.0) leftColor = "#641e64", rightColor = "#c80000";
    else if(progress >= 3.0) leftColor = "#641e64", rightColor = "#641e64";


    ui->progressbar->setStyleSheet(QString("QProgressBar{ border: 1px solid #000; border-radius: 2px; }")
                                   .append("QProgressBar::chunk{border-radius: 2px;")
                                       .append("background-color: QLinearGradient(x1: 0,y1: 0,x2: 1, y2: 0,")
                                           .append("stop: 0 " + leftColor + ", stop: " + lowerbborder + " " + leftColor + ",")
                                           .append("stop: " + upperbborder + " " + rightColor + ", stop: 1 " + rightColor + ");}"));
    ui->progressbar->setValue(2000);
}


void FightclubDepartment::propagateProblemsList(int problem) {
    QAbstractTableModel* model = probadapt->getProblemList(problem);

    ui->problemcombobox->setModel(model);

    ui->problemcombobox->setEnabled(model->rowCount() > 1);
    ui->toggleEditProblemBttn->setEnabled(model->rowCount() > 1);
    ui->toggleEditProblemBttn->setIcon(QIcon(":/breeze-icons/dialog-ok-apply-16.svg"));

    if(problem < 0) ui->problemcombobox->setCurrentIndex(-1);
}

void FightclubDepartment::editProblemBttnToggled() {
    ui->problemcombobox->setEnabled(!ui->problemcombobox->isEnabled());
    ui->toggleEditProblemBttn->setIcon(QIcon(QString(":/breeze-icons/")
        .append(ui->problemcombobox->isEnabled()? "dialog-ok-apply" : "document-edit").append("-16.svg")));
}


void FightclubDepartment::performersChanged(QString rep, QString opp, QString rev) {
    ui->repcombobox->setModel(teamadapt->getPerformersList(rep, true));
    ui->oppcombobox->setModel(teamadapt->getPerformersList(opp, true));
    ui->revcombobox->setModel(teamadapt->getPerformersList(rev, true));

    repcomboboxinit = rep; oppcomboboxinit = opp; revcomboboxinit = rev;

    ui->repcombobox->setEnabled(ui->repcombobox->model()->rowCount() > 1);
    ui->toggleEditRepBttn->setEnabled(ui->revcombobox->model()->rowCount() > 1);
    ui->toggleEditRepBttn->setIcon(QIcon(":/breeze-icons/dialog-ok-apply-16.svg"));

    ui->oppcombobox->setEnabled(ui->oppcombobox->model()->rowCount() > 1);
    ui->toggleEditOppBttn->setEnabled(ui->oppcombobox->model()->rowCount() > 1);
    ui->toggleEditOppBttn->setIcon(QIcon(":/breeze-icons/dialog-ok-apply-16.svg"));

    ui->revcombobox->setEnabled(ui->revcombobox->model()->rowCount() > 1);
    ui->toggleEditRevBttn->setEnabled(ui->repcombobox->model()->rowCount() > 1);
    ui->toggleEditRevBttn->setIcon(QIcon(":/breeze-icons/dialog-ok-apply-16.svg"));
}


void FightclubDepartment::updateReporterModel(int index) {
    if((repcomboboxinit == nullptr) || (index == 0)) return;

    ui->repcombobox->setModel(teamadapt->getPerformersList(repcomboboxinit, false));
    ui->repcombobox->setCurrentIndex(index -1);
    repcomboboxinit = nullptr;
}

void FightclubDepartment::editRepBttnToggled() {
    ui->repcombobox->setEnabled(!ui->repcombobox->isEnabled());
    ui->toggleEditRepBttn->setIcon(QIcon(QString(":/breeze-icons/")
        .append(ui->repcombobox->isEnabled()? "dialog-ok-apply" : "document-edit").append("-16.svg")));
}


void FightclubDepartment::updateOpponentModel(int index) {
    if((oppcomboboxinit == nullptr) || (index == 0)) return;

    ui->oppcombobox->setModel(teamadapt->getPerformersList(oppcomboboxinit, false));
    ui->oppcombobox->setCurrentIndex(index -1);
    oppcomboboxinit = nullptr;
}

void FightclubDepartment::editOppBttnToggled() {
    ui->oppcombobox->setEnabled(!ui->oppcombobox->isEnabled());
    ui->toggleEditOppBttn->setIcon(QIcon(QString(":/breeze-icons/")
        .append(ui->oppcombobox->isEnabled()? "dialog-ok-apply" : "document-edit").append("-16.svg")));
}


void FightclubDepartment::updateReviewerModel(int index) {
    if((revcomboboxinit == nullptr) || (index == 0)) return;

    ui->revcombobox->setModel(teamadapt->getPerformersList(revcomboboxinit, false));
    ui->revcombobox->setCurrentIndex(index -1);
    revcomboboxinit = nullptr;
}

void FightclubDepartment::editRevBttnToggled() {
    ui->revcombobox->setEnabled(!ui->revcombobox->isEnabled());
    ui->toggleEditRevBttn->setIcon(QIcon(QString(":/breeze-icons/").append(ui->revcombobox->isEnabled()?
                                                     "dialog-ok-apply" : "document-edit").append("-16.svg")));
}







// BROADCAST TAB ------------------------------------------------------------------------

void FightclubDepartment::updateBcastIPBoxes() {
    ui->customippt1->setEnabled(ui->selectIPCombobox->itemData(ui->selectIPCombobox->currentIndex()).toInt() == IP_CUSTOM);
    ui->customippt2->setEnabled(ui->selectIPCombobox->itemData(ui->selectIPCombobox->currentIndex()).toInt() == IP_CUSTOM);
    ui->customippt3->setEnabled(ui->selectIPCombobox->itemData(ui->selectIPCombobox->currentIndex()).toInt() == IP_CUSTOM);
    ui->customippt4->setEnabled(ui->selectIPCombobox->itemData(ui->selectIPCombobox->currentIndex()).toInt() == IP_CUSTOM);

    int tmp1, tmp2, tmp3, tmp4;

    switch(ui->selectIPCombobox->itemData(ui->selectIPCombobox->currentIndex()).toInt()) {
        case IP_LOCAL:  tmp1 = 127, tmp2 =   0, tmp3 =   0, tmp4 =   1; break;
        case IP_BCAST:  tmp1 = 255, tmp2 = 255, tmp3 = 255, tmp4 = 255; break;
        case IP_CUSTOM: tmp1 = 192, tmp2 = 168, tmp3 =   0, tmp4 =   1; break;
    }

    ui->customippt1->setValue(tmp1);
    ui->customippt2->setValue(tmp2);
    ui->customippt3->setValue(tmp3);
    ui->customippt4->setValue(tmp4);
}

void FightclubDepartment::applyBcastSettings() {
    QString bcastIP = QString::number(ui->customippt1->value()) + "."
                    + QString::number(ui->customippt2->value()) + "."
                    + QString::number(ui->customippt3->value()) + "."
                    + QString::number(ui->customippt4->value());

    bcastsrv->setBroadcastAddress(bcastIP);
    bcastsrv->setBroadcastPort(ui->bcastportsel->value());
    bcastsrv->setSignature(ui->bcastidsel->value());
}







// FIGHTCLUB TAB ------------------------------------------------------------------------

void FightclubDepartment::openStagesFile() {
    if(continueAndInit()) {
        QFileDialog *selectStagesFileDialog
                = new QFileDialog(this, "Select a stages file", previousPath, "Fightclub stages files (*.fcstages)");

        if(selectStagesFileDialog->exec()) {
            QString file = selectStagesFileDialog->selectedFiles().value(0);

            FilePropertyParser *fpp = new FilePropertyParser(file);

            if(!(fpp->getFileType() == nullptr || fpp->getFileType().contains("stages", Qt::CaseInsensitive))) {
                QMessageBox::critical(this, "Wrong file format",
                                      "You requested a stages file, but " + QFileInfo(file).fileName() + " is a " + fpp->getFileType() + " file.");
                return;
            }

            lstadapt->loadStagesListFromFile(file);
            previousPath = selectStagesFileDialog->directory().absolutePath();
            ui->stagesFileTitle->setText(fpp->getTitle());
            ui->stagesFileDescr->setText(fpp->getDescription());
        }
    }
}


void FightclubDepartment::openPhasesFile() {
    if(continueAndInit()) {
        QFileDialog *selectPhasesFileDialog
                = new QFileDialog(this, "Select a phases file", previousPath, "Fightclub phases files (*.fcphases)");

        if(selectPhasesFileDialog->exec()) {
            QString file = selectPhasesFileDialog->selectedFiles().value(0);

            FilePropertyParser *fpp = new FilePropertyParser(file);

            if(!(fpp->getFileType() == nullptr || fpp->getFileType().contains("phases", Qt::CaseInsensitive))) {
                QMessageBox::critical(this, "Wrong file format",
                                      "You requested a phases file, but " + QFileInfo(file).fileName() + " is a " + fpp->getFileType() + " file.");
                return;
            }

            lstadapt->loadPhasesListFromFile(file);
            previousPath = selectPhasesFileDialog->directory().absolutePath();
            ui->phasesFileTitle->setText(fpp->getTitle());
            ui->phasesFileDescr->setText(fpp->getDescription());
        }
    }
}


void FightclubDepartment::openProblemsFile() {
    if(continueAndInit()) {
        QFileDialog *selectProblemsFileDialog
                = new QFileDialog(this, "Select a problems file", previousPath, "Fightclub problems files (*.fcproblems)");

        if(selectProblemsFileDialog->exec()) {
            QString file = selectProblemsFileDialog->selectedFiles().value(0);

            FilePropertyParser *fpp = new FilePropertyParser(file);

            if(!(fpp->getFileType() == nullptr || fpp->getFileType().contains("problems", Qt::CaseInsensitive))) {
                QMessageBox::critical(this, "Wrong file format",
                                      "You requested a problems file, but " + QFileInfo(file).fileName() + " is a " + fpp->getFileType() + " file.");
                return;
            }

            probadapt->loadProblemsFromFile(file);
            previousPath = selectProblemsFileDialog->directory().absolutePath();

            ui->problemsFileTitle->setText((probadapt->getProblemCount() > 0)? fpp->getTitle() : "No problems loaded");
            ui->problemsFileDescr->setText((probadapt->getProblemCount() > 0)? fpp->getDescription() : " ");
            ui->unloadProblemsFile->setEnabled(probadapt->getProblemCount() > 0);

            lstadapt->initialize();
        }
    }
}


void FightclubDepartment::openTeamsFile() {
    if(continueAndInit()) {
        QFileDialog *selectTeamsFileDialog
                = new QFileDialog(this, "Select a teams file", previousPath, "Fightclub teams files (*.fcteams)");

        if(selectTeamsFileDialog->exec()) {
            QString file = selectTeamsFileDialog->selectedFiles().value(0);

            FilePropertyParser *fpp = new FilePropertyParser(file);

            if(!(fpp->getFileType() == nullptr || fpp->getFileType().contains("teams", Qt::CaseInsensitive))) {
                QMessageBox::critical(this, "Wrong file format",
                                      "You requested a teams file, but " + QFileInfo(file).fileName() + " is a " + fpp->getFileType() + " file.");
                return;
            }

            teamadapt->loadTeamsFromFile(file);
            previousPath = selectTeamsFileDialog->directory().absolutePath();

            ui->teamsFileTitle->setText((teamadapt->getTeamCount() > 0)? fpp->getTitle() : "No teams loaded");
            ui->teamsFileDescr->setText((teamadapt->getTeamCount() > 0)? fpp->getDescription() : " ");
            ui->unloadTeamsFile->setEnabled(teamadapt->getTeamCount() > 0);

            lstadapt->initialize();
        }
    }
}


void FightclubDepartment::unloadStagesFile()   { if(continueAndInit()) lstadapt->unloadStagesList(); }
void FightclubDepartment::unloadPhasesFile()   { if(continueAndInit()) lstadapt->unloadPhasesList(); }

void FightclubDepartment::unloadProblemsFile() {
    if(continueAndInit()) {
        probadapt->unloadProblemsList();
        ui->problemsFileTitle->setText("No problems loaded");
        ui->problemsFileDescr->setText(" ");
        ui->unloadProblemsFile->setEnabled(false);
        lstadapt->initialize();
    }
}

void FightclubDepartment::unloadTeamsFile() {
    if(continueAndInit()) {
        teamadapt->unloadTeams();
        ui->teamsFileTitle->setText("No teams loaded");
        ui->teamsFileDescr->setText(" ");
        ui->unloadTeamsFile->setEnabled(false);
        lstadapt->initialize();
    }
}







// INTERNAL STUFF -----------------------------------------------------------------------

void FightclubDepartment::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_Shift: emit switchStages(true);       break;
        default:            QWidget::keyPressEvent(event); break;
    }
}

void FightclubDepartment::keyReleaseEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_Shift: emit switchStages(false);      break;
        default:            QWidget::keyPressEvent(event); break;
    }
}



void FightclubDepartment::activateFullscreen(bool fscreen) {
    if(!toggleFscreenEnabled) return;

    if(fscreen) setWindowState(Qt::WindowFullScreen);
    else if(windowState() == Qt::WindowFullScreen) setWindowState(Qt::WindowMaximized);

    ui->actionFullscreen->setShortcut(fscreen? QKeySequence(Qt::Key_Escape)
                                             : QKeySequence(Qt::ControlModifier + Qt::Key_F));
}



void FightclubDepartment::closeEvent(QCloseEvent *event) {
    if(!exitEnabled) {
        QMessageBox::information(this, "Information", "Closing Fightclub Department is currently disabled.");
        event->ignore();
        return;
    }

    if(QMessageBox::warning(this,
            "Confirmation requested",
            "Do you really want to close Fightclub Department?",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
        emit closeAllClockWindows();
        event->accept();
    } else event->ignore();
}
