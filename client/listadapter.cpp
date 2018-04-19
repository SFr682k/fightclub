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


#include "listadapter.h"

#include <QFile>
#include <QTextStream>

#include <QDebug>

ListAdapter::ListAdapter(QObject *parent) : QObject(parent) {
    stagelistmodel = new StageListModel();
    phaselistmodel = new PhaseListModel();
    currentStage = -1;
    currentPhase = -1;
}


void ListAdapter::setUpPhaseSwitchingButtons() {
    if(phaselistmodel->rowCount() == 0) {
        emit enablePrevPhaseButton(false);
        emit enableNextPhaseButton(false);
        return;
    }

    if(stagelistmodel->rowCount() > 0) {
        emit enablePrevPhaseButton(!((currentStage <= 0) && (currentPhase < 0)));

        Stage cStage = stagelistmodel->getStageList().value(currentStage);
        bool currentStageIsRclk = (cStage.getRoomclockstage() != nullptr);

        if(((currentStage >= stagelistmodel->rowCount() -1) && (currentPhase >= phaselistmodel->rowCount() -1))
            || ((currentStage >= stagelistmodel->rowCount() -1) && currentStageIsRclk))
                emit enableNextPhaseButton(false);
        else    emit enableNextPhaseButton(true);

        emit currentStageIsRCS(currentStageIsRclk);
    } else {
        emit enablePrevPhaseButton(currentPhase > 0);
        emit enableNextPhaseButton(currentPhase < phaselistmodel->rowCount() -1);

        emit currentStageIsRCS(false);
    }
}


void ListAdapter::prevPhase() {
    if((currentStage < 1) && (currentPhase < 0)) {
        emit endOfStage();
        if(stagelistmodel->rowCount() > 0) {
            currentStage =  0;
            currentPhase = -1;
        } else currentPhase = 0;

        setPhaseProperties();
        return;
    }


    if(currentPhase >= 0) {
        Phase phaseBeingLeft = phaselistmodel->getPhasesList().value(currentPhase);
        if(phaseBeingLeft.getRoomclock()) emit resetTime(); }
    else emit resetTime();

    if((currentPhase > 0) || ((currentPhase == 0) && (stagelistmodel->rowCount() > 0)))
        currentPhase--;
    else if(stagelistmodel->rowCount() > 0) {
        currentStage--;

        Stage cStage = stagelistmodel->getStageList().value(currentStage);
        bool currentStageIsRclk = (cStage.getRoomclockstage() != nullptr);

        if(!currentStageIsRclk) currentPhase = phaselistmodel->rowCount() -1;
        else                    currentPhase = -1;

        stagelistmodel->setHighlightedRow(currentStage);
        emit currentStageChanged(currentStage);
    }

    phaselistmodel->setHighlightedRow(currentPhase);
    emit currentPhaseChanged(currentPhase);

    setUpPhaseSwitchingButtons();

    handleOvertime(-120000); // erase autoadvance countdown

    if(currentPhase > -1) {
        QTime tmptime = QTime(0,0,0);

        Phase phase = phaselistmodel->getPhasesList().value(currentPhase);
        emit roomClockChanged(phase.getRoomclock());

        if(!phase.getCarry()) emit resetTime();

        emit phaseNameChanged(phase.getName());
        emit maximumTimeChanged(tmptime.msecsTo(phase.getDuration()));
        emit overtimeChanged(tmptime.msecsTo(phase.getOvertime()));
    } else {
        emit roomClockChanged(true);

        Stage cStage = stagelistmodel->getStageList().value(currentStage);
        bool currentStageIsRclk = (cStage.getRoomclockstage() != nullptr);

        if(!currentStageIsRclk) {
            QString stageoview;
            stageoview = cStage.getLabel() + " — ";
            stageoview = stageoview + "FIXME: Show a stage overview (Rep <> Opp <> Rev: Problem)";
            emit phaseNameChanged(stageoview);
        } else emit phaseNameChanged(cStage.getRoomclockstage());
    }


    setPhaseProperties();
}


void ListAdapter::nextPhase() {
    if(phaselistmodel->rowCount() == 0) {
        currentPhase = -1;
        emit phaseNameChanged("CRITICAL ERROR: No phases specified");
        return;
    }

    if(stagelistmodel->rowCount() > 0) {
        Stage cStage = stagelistmodel->getStageList().value(currentStage);
        bool currentStageIsRclk = (cStage.getRoomclockstage() != nullptr);

        if((currentStage < stagelistmodel->rowCount() -1)
                || ((currentStage == stagelistmodel->rowCount() -1)
                    && (currentPhase < phaselistmodel->rowCount()-1))) {
            if((currentPhase < phaselistmodel->rowCount() -1) && (currentStage != -1)
                    && !currentStageIsRclk) currentPhase++;
            else if (currentStage < stagelistmodel->rowCount() -1) {
                currentStage++;
                currentPhase = -1;
            }
        }

        stagelistmodel->setHighlightedRow(currentStage);
        emit currentStageChanged(currentStage);
    } else if(currentPhase < phaselistmodel->rowCount() -1)
        currentPhase++;

    phaselistmodel->setHighlightedRow(currentPhase);
    emit currentPhaseChanged(currentPhase);

    setUpPhaseSwitchingButtons();

    handleOvertime(-120000); // erase autoadvance countdown

    if(currentPhase > -1) {
        QTime tmptime = QTime(0,0,0);

        Phase phase     = phaselistmodel->getPhasesList().value(currentPhase);

        emit roomClockChanged(phase.getRoomclock());

        Phase prevphase;
        if(currentPhase > 0) {
            prevphase = phaselistmodel->getPhasesList().value(currentPhase - 1);

            if(!prevphase.getCarry()) {
                if(prevphase.getOCarry()) emit getElapsedOvertime();
                else                      emit resetTime();
            }
        } else emit resetTime();

        emit phaseNameChanged(phase.getName());
        emit maximumTimeChanged(tmptime.msecsTo(phase.getDuration()));
        emit overtimeChanged(tmptime.msecsTo(phase.getOvertime()));
    } else {
        emit roomClockChanged(true);

        Stage cStage = stagelistmodel->getStageList().value(currentStage);
        bool currentStageIsRclk = (cStage.getRoomclockstage() != nullptr);

        if(!currentStageIsRclk) {
            QString stageoview;
            stageoview = cStage.getLabel() + " — ";
            stageoview = stageoview + "FIXME: Show a stage overview (Rep <> Opp <> Rev: Problem)";
            emit phaseNameChanged(stageoview);
        } else emit phaseNameChanged(cStage.getRoomclockstage());
    }

    setPhaseProperties();
}


void ListAdapter::setPhaseProperties() {
    if(currentPhase < 0) {
        emit currPhaseAAdv(false);
        emit currPhaseCarry(false);
        emit currPhaseOCarry(false);
    } else {
        Phase currPhase = phaselistmodel->getPhasesList().value(currentPhase);
        emit currPhaseAAdv(currPhase.getAutoadvance());
        emit currPhaseCarry(currPhase.getCarry());
        emit currPhaseOCarry(currPhase.getOCarry());
    }



    if(((currentPhase < 0) && (currentStage <= 0)) // 1) information phase of 1st stage
            || (currentPhase == 0)) { // 2) first phase of a stage
        /**
         * The previous phase is …
         *   1) not present or
         *   2) a phase displaying information about the current phase
         * so there are no special phase properties.
         */
        emit prevPhaseAAdv(false);
        emit prevPhaseCarry(false);
        emit prevPhaseOCarry(false);
    } else if(currentPhase > 0) {
        /**
         * There is a previous phase inside the current stage not being
         * the phase displaying the stage information.
         */

        Phase prevPhase = phaselistmodel->getPhasesList().value(currentPhase-1);
        emit prevPhaseAAdv(prevPhase.getAutoadvance());
        emit prevPhaseCarry(prevPhase.getCarry());
        emit prevPhaseOCarry(prevPhase.getOCarry());
    } else if(currentPhase < 0) {
        /**
         * We’re inside the phase displaying information about the current stage
         * (and a previous stage exists). The previous phase is either
         *   1) the last phase of the previous stage or
         *   2) the phase displaying information about the last stage
         */

        Stage prevStage = stagelistmodel->getStageList().value(currentStage -1);
        bool prevStageIsRclk = (prevStage.getRoomclockstage() != nullptr);

        if(!prevStageIsRclk) {
            // The previous stage is no roomclock stage; so we have to check for
            // the last phase’s properties
            Phase lastPhase = phaselistmodel->getPhasesList()
                    .value(phaselistmodel->rowCount() -1);
            emit prevPhaseAAdv(lastPhase.getAutoadvance());
            emit prevPhaseCarry(lastPhase.getCarry());
            emit prevPhaseOCarry(lastPhase.getOCarry());
        } else {
            // Previous stage is a roomclock stage: previous phase is roomclock phase
            // Therefore, there are no special phase properties
            emit prevPhaseAAdv(false);
            emit prevPhaseCarry(false);
            emit prevPhaseOCarry(false);
        }
    }
}


void ListAdapter::handleOvertime(int overtimed) {
    Phase phase = phaselistmodel->getPhasesList().value(currentPhase);

    if(!phase.getAutoadvance() || overtimed < -60000) {
        emit overtimeCountdownChanged("    ");
        return;
    }

    if(overtimed < 0) {
        QTime timeUntilAAdv = QTime(0,0,0);
        timeUntilAAdv = timeUntilAAdv.addMSecs(-overtimed);
        int tsec = timeUntilAAdv.msec() / 100;
        QString dispCDownValue = timeUntilAAdv.toString("s.") + QString::number(tsec);
        emit overtimeCountdownChanged(dispCDownValue);
        return;
    }

    nextPhase();

    QTime temp = QTime(0,0,0);
    int carriedotime = temp.msecsTo(phase.getOvertime());
    if(!phase.getCarry() && phase.getOCarry()) emit elapsedTimeChanged(carriedotime);
}







// ALGORITHMS LOADING EXTERNAL FILES --------------------------------------------------------------

int ListAdapter::loadStagesListFromFile(QString path) {
    QList<Stage> tmplist;
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 1;

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList splitline = line.split('\t');

        int problem;
        QString repID = nullptr, oppID = nullptr, revID = nullptr;

        if(splitline.size() < 3) {
            QString entry = splitline.value(0);

            if(entry.length() > 0
                    && !(entry.startsWith(' ', Qt::CaseInsensitive))
                    && !(entry.contains("</>")))
                tmplist.append(Stage(splitline.value(0)));
        } else if(splitline.size() > 2) {
            QString label = splitline.value(0);
            problem = splitline.value(1).toInt();

            repID = splitline.value(2);
            if(splitline.size() > 3) oppID = splitline.value(4);
            if(splitline.size() > 4) revID = splitline.value(4);

            tmplist.append(Stage(nullptr, label, problem, repID, oppID, revID));
        }
    }

    stagelistmodel = new StageListModel(tmplist);
    emit stageListModelChanged(stagelistmodel);
    file.close();

    setUpPhaseSwitchingButtons();
    initialize();

    return 0;
}


int ListAdapter::loadPhasesListFromFile(QString path) {
    QList<Phase> tmplist;
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 1;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QTime tmptime = QTime(0,0,0);
        QString line = in.readLine();
        QStringList splitline = line.split('\t');

        bool rpperf = false, opperf = false, rvperf = false;
        bool aadvnc = false, tcarry = false, ocarry = false, rclock = false;

        if(splitline.size() > 3) {
            rpperf = splitline.value(3).contains("rep", Qt::CaseInsensitive);
            opperf = splitline.value(3).contains("opp", Qt::CaseInsensitive);
            rvperf = splitline.value(3).contains("rev", Qt::CaseInsensitive);
        }

        if(splitline.size() > 4) {
            aadvnc = splitline.value(4).contains('a', Qt::CaseInsensitive);
            tcarry = splitline.value(4).contains('c', Qt::CaseInsensitive);
            ocarry = splitline.value(4).contains('o', Qt::CaseInsensitive);
            rclock = splitline.value(4).contains('r', Qt::CaseInsensitive);
        }

        if(splitline.size() >= 3) tmplist.append(Phase(
                               tmptime.addMSecs(splitline.value(0).toInt()*1000),
                               tmptime.addMSecs(splitline.value(1).toInt()*1000),
                               splitline.value(2),
                               rpperf, opperf, rvperf,
                               aadvnc, tcarry, ocarry, rclock));
        else {}// We're reading a comment or some other kind of line. Don't do anything.
    }

    phaselistmodel = new PhaseListModel(tmplist);
    emit phaseListModelChanged(phaselistmodel);
    file.close();

    setUpPhaseSwitchingButtons();
    initialize();

    return 0;
}


void ListAdapter::unloadStagesList() {
    QList<Stage> tmplist;
    stagelistmodel = new StageListModel(tmplist);
    emit stageListModelChanged(stagelistmodel);
    initialize();
}


void ListAdapter::unloadPhasesList() {
    QList<Phase> tmplist;
    phaselistmodel = new PhaseListModel(tmplist);
    emit phaseListModelChanged(phaselistmodel);
    initialize();
}


void ListAdapter::initialize() {
    currentStage = -1;
    currentPhase = -1;

    stagelistmodel->setHighlightedRow(currentStage);
    phaselistmodel->setHighlightedRow(currentPhase);

    if(stagelistmodel->rowCount() > 0) emit currentStageChanged(-1);
    if(phaselistmodel->rowCount() > 0) emit currentPhaseChanged(-1);

    emit prevPhaseAAdv(false);
    emit prevPhaseCarry(false);
    emit prevPhaseOCarry(false);
    emit currPhaseAAdv(false);
    emit currPhaseCarry(false);
    emit currPhaseOCarry(false);

    emit phaseNameChanged("Click the “Start” button to initiate the first phase");
    emit roomClockChanged(true);
    setUpPhaseSwitchingButtons();
    emit forceInit();
}



int ListAdapter::getCurrentStage() { return currentStage; }
int ListAdapter::getCurrentPhase() { return currentPhase; }

