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

#include <QStringListModel>
#include <QObject>


class ListAdapter : public QObject
{
    Q_OBJECT
public:
    explicit ListAdapter(QObject *parent = nullptr);
    int loadNanoPhasesListFromFile(QString);
    void setUpPhaseSwitchingButtons();
    int getCurrentPhase();

private:
    PhaseListModel* phaselistmodel;
    bool skipToBoundaries;
    int currentPhase;

signals:
    void phaseListModelChanged(QAbstractTableModel*);
    void currentPhaseChanged(int);
    void enablePrevPhaseButton(bool);
    void enableNextPhaseButton(bool);


    void carryElapsedOvertime();
    void resetTime();
    void elapsedTimeChanged(int);
    void maximumTimeChanged(int);
    void overtimeChanged(int);
    void phaseLabelChanged(QString);
    void roomClockChanged(bool);
    void endOfStage();


    void forceInit();

public slots:
    void fwd();
    void bwd();
    void skipToBounds(bool);

    void firstPhase();
    void prevPhase();
    void nextPhase();
    void lastPhase();

    void handleOvertime(int);

    void unloadPhasesList();

    void initialize();

private slots:
    void onPhaseChanges(int);
};

#endif // LISTADAPTER_H
