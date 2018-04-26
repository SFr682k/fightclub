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


#ifndef LISTADAPTER_H
#define LISTADAPTER_H


#include "phaselistmodel.h"
#include "stagelistmodel.h"
#include "teamadapter.h"

#include <QStringListModel>
#include <QObject>


class ListAdapter : public QObject
{
    Q_OBJECT
public:
    explicit ListAdapter(QObject *parent = nullptr);
    void setTeamAdapter(TeamAdapter* teamadapt = nullptr);
    int loadStagesListFromFile(QString);
    int loadPhasesListFromFile(QString);
    void setUpPhaseSwitchingButtons();
    int getCurrentStage();
    int getCurrentPhase();

private:
    void setPhaseProperties();
    StageListModel* stagelistmodel;
    PhaseListModel* phaselistmodel;
    TeamAdapter* teamadapter;
    int currentStage;
    int currentPhase;
    QString getStageOverview(Stage);

signals:
    void stageListModelChanged(QAbstractTableModel*);
    void currentStageChanged(int);
    void phaseListModelChanged(QAbstractTableModel*);
    void currentPhaseChanged(int);
    void enablePrevPhaseButton(bool);
    void enableNextPhaseButton(bool);
    void currentStageIsRCS(bool);

    void currentProblemChanged(int);

    void currentPerformersChanged(QString, QString, QString);

    void prevPhasePropsChanged(bool,bool,bool);
    void currPhasePropsChanged(bool,bool,bool);

    void getElapsedOvertime();
    void resetTime();
    void elapsedTimeChanged(int);
    void maximumTimeChanged(int);
    void overtimeChanged(int);
    void overtimeCountdownChanged(QString);
    void phaseNameChanged(QString);
    void roomClockChanged(bool);
    void endOfStage();

    void forceInit();

public slots:
    void prevPhase();
    void nextPhase();
    void handleOvertime(int);

    void unloadStagesList();
    void unloadPhasesList();

    void initialize();

private slots:
    void onStageChanges(int);
    void onPhaseChanges(int);
};

#endif // LISTADAPTER_H
