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
#include <QKeyEvent>

#include "aboutdialog.h"
#include "broadcastserver.h"
#include "clkwindowsettings.h"
#include "clockwindow.h"
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
    ClockWindow *clockwindow;
    ListAdapter *lstadapt;
    PhasePBar *phpbar;
    ProblemAdapter *probadapt;
    SetTimeDialog *settimedlg;
    TeamAdapter *teamadapt;

    QString repcomboboxinit, oppcomboboxinit, revcomboboxinit;

    QString previousPath;

    bool exitEnabled, toggleFscreenEnabled;

    bool continueAndInit();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

signals:
    void switchStages(bool);

private slots:
    void openClockWindow();
    void clockWindowClosed();

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

    void performersChanged(QString, QString, QString);
    void updateReporterModel(int);
    void updateOpponentModel(int);
    void updateReviewerModel(int);

    void setBroadcastIP();
    void setBroadcastPort();
    void setBroadcastID();
    void checkCustomIPRbttn();

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
