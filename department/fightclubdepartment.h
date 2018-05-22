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


#ifndef FIGHTCLUBDEPARTMENT_H
#define FIGHTCLUBDEPARTMENT_H

#include <QMainWindow>
#include <QAbstractTableModel>
#include <QItemSelection>
#include <QKeyEvent>

#include "aboutdialog.h"
#include "broadcastserver.h"
#include "clkwindowsettings.h"
#include "listadapter.h"
#include "phasepbar.h"
#include "problemadapter.h"
#include "settimedialog.h"
#include "teamadapter.h"

namespace Ui {
class FightclubDepartment;
}

class FightclubDepartment : public QMainWindow
{
    Q_OBJECT

public:
    explicit FightclubDepartment(QWidget *parent = 0);
    ~FightclubDepartment();

private:
    Ui::FightclubDepartment *ui;
    AboutDialog *aboutdlg;
    BroadcastServer *bcastsrv;
    ClkWindowSettings *clkwindowsettings;
    ListAdapter *lstadapt;
    PhasePBar *phpbar;
    ProblemAdapter *probadapt;
    SetTimeDialog *settimedlg;
    TeamAdapter *teamadapt;

    QString repcomboboxinit, oppcomboboxinit, revcomboboxinit;

    QString previousPath;

    bool exitEnabled, toggleFscreenEnabled;

    QAbstractTableModel *bcasttablemodel;
    enum {IP_BCAST, IP_LOCAL, IP_CUSTOM};

    bool continueAndInit();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void switchStages(bool);
    void closeAllClockWindows();

private slots:
    void openClockWindow();

    void switchBetweenStages(bool);
    void toggleStartStopBttn();
    void openSetTimeDialog();
    void propagateStagesList(QAbstractTableModel*);
    void scrollToSelectedStage(int);
    void propagatePhasesList(QAbstractTableModel*);
    void scrollToSelectedPhase(int);

    void setPrevPhaseProps(bool,bool,bool);
    void setCurrPhaseProps(bool,bool,bool);

    void propagateProblemsList(int);
    void editProblemBttnToggled();

    void performersChanged(QString, QString, QString);
    void updateReporterModel(int);
    void editRepBttnToggled();
    void updateOpponentModel(int);
    void editOppBttnToggled();
    void updateReviewerModel(int);
    void editRevBttnToggled();

    void propagateBroadcastList(QAbstractTableModel*);
    void bcastSelectionChanged(QItemSelection, QItemSelection);
    void updateBcastIPBoxes();
    void applyBcastSettings();
    void addBcast();
    void deleteBcast();

    void openStagesFile();
    void unloadStagesFile();
    void openPhasesFile();
    void unloadPhasesFile();
    void openProblemsFile();
    void unloadProblemsFile();
    void openTeamsFile();
    void unloadTeamsFile();

    void initialize();

    void activateFullscreen(bool);


public slots:
    void setPhaseProgress(double);

};

#endif // FIGHTCLUBDEPARTMENT_H
