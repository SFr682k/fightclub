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



ListAdapter::ListAdapter(QObject *parent) : QObject(parent) {
    phaselistmodel = new PhaseListModel();
    currentPhase = -1;
    skipToBoundaries = false;

    connect(this, SIGNAL(currentPhaseChanged(int)), this, SLOT(onPhaseChanges(int)));
}



void ListAdapter::setUpPhaseSwitchingButtons() {
    if(phaselistmodel->rowCount() == 0) {
        emit enablePrevPhaseButton(false);
        emit enableNextPhaseButton(false);
        return;
    }

    emit enablePrevPhaseButton(currentPhase > 0);
    emit enableNextPhaseButton(currentPhase < phaselistmodel->rowCount() -1);
}



void ListAdapter::fwd() {
    if(!skipToBoundaries) nextPhase();
    else                  lastPhase();
}

void ListAdapter::bwd() {
    if(!skipToBoundaries) prevPhase();
    else                  firstPhase();
}

void ListAdapter::skipToBounds(bool skToBounds) {
    skipToBoundaries = skToBounds;
}



void ListAdapter::firstPhase() {
    if(currentPhase > 0) {
        currentPhase = 0;
        emit resetTime();
        emit currentPhaseChanged(currentPhase);
    }
}


void ListAdapter::prevPhase() {
    int leftFromPhase = currentPhase;

    if(currentPhase > 0) {
        currentPhase--;

        if(leftFromPhase > 0) {
            if(phaselistmodel->getPhasesList().value(leftFromPhase).getRoomclock())
                emit resetTime();
            else if(!(phaselistmodel->getPhasesList().value(currentPhase).getCarry()))
                emit resetTime();
        } else emit resetTime();

        emit currentPhaseChanged(currentPhase);
    } else emit endOfStage();
}


void ListAdapter::nextPhase() {
    if(phaselistmodel->rowCount() == 0) {
        currentPhase = -1;
        emit phaseLabelChanged("CRITICAL ERROR: No phases specified");
        return;
    }

    if(currentPhase < phaselistmodel->rowCount() -1) {
        currentPhase++;

        if(currentPhase > 0) {
            Phase prevphase = phaselistmodel->getPhasesList().value(currentPhase - 1);

            if(!prevphase.getCarry()) {
                if(prevphase.getOCarry()) emit carryElapsedOvertime();
                else                      emit resetTime();
            }
        } else emit resetTime();

        emit currentPhaseChanged(currentPhase);
    } else emit endOfStage();
}


void ListAdapter::lastPhase() {
    if(phaselistmodel->rowCount() == 0) {
        currentPhase = -1;
        emit phaseLabelChanged("CRITICAL ERROR: No phases specified");
        return;
    }

    currentPhase = phaselistmodel->rowCount() -1;
    emit currentPhaseChanged(currentPhase);
    emit resetTime();
}


void ListAdapter::onPhaseChanges(int phasenr) {
    Q_UNUSED(phasenr);

    phaselistmodel->setHighlightedRow(currentPhase);

    setUpPhaseSwitchingButtons();


    if(currentPhase > -1) {
        QTime tmptime = QTime(0,0,0);

        Phase phase     = phaselistmodel->getPhasesList().value(currentPhase);

        emit roomClockChanged(phase.getRoomclock());

        emit phaseLabelChanged(phase.getName());
        emit maximumTimeChanged(tmptime.msecsTo(phase.getDuration()));
        emit overtimeChanged(tmptime.msecsTo(phase.getOvertime()));
    }
}





void ListAdapter::handleOvertime(int overtimed) {
    if((currentPhase < 0) || (phaselistmodel->rowCount() == 0)) return;

    Phase phase = phaselistmodel->getPhasesList().value(currentPhase);

    if(!phase.getAutoadvance() || !(overtimed > 0)) return;

    nextPhase();

    QTime temp = QTime(0,0,0);
    int carriedotime = temp.msecsTo(phase.getOvertime());
    if(!phase.getCarry() && phase.getOCarry()) emit elapsedTimeChanged(carriedotime);
}






// ALGORITHM FOR LOADING EXTERNAL FILES -----------------------------------------------------------

int ListAdapter::loadPhasesListFromFile(QString path) {
    QList<Phase> tmplist;
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 1;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QTime tmptime = QTime(0,0,0);
        QString line = in.readLine();
        QStringList splitline = line.split('\t');

        bool aadvnc = false, tcarry = false, ocarry = false, rclock = false;



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


void ListAdapter::unloadPhasesList() {
    QList<Phase> tmplist;
    phaselistmodel = new PhaseListModel(tmplist);
    emit phaseListModelChanged(phaselistmodel);
    initialize();
}







void ListAdapter::initialize() {
    currentPhase = -1;

    phaselistmodel->setHighlightedRow(currentPhase);

    if(phaselistmodel->rowCount() > 0) emit currentPhaseChanged(-1);


    emit phaseLabelChanged("Click the “Start” button to initiate the first phase");
    emit roomClockChanged(true);
    setUpPhaseSwitchingButtons();
    emit forceInit();
}



int ListAdapter::getCurrentPhase() { return currentPhase; }

