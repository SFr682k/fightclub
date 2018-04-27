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


#ifndef FIGHTCLUBCLIENT_H
#define FIGHTCLUBCLIENT_H

#include <QMainWindow>
#include <QAbstractTableModel>

#include "aboutdialog.h"
#include "broadcastserver.h"
#include "clockwindow.h"
#include "listadapter.h"
#include "phasepbar.h"
#include "problemadapter.h"
#include "settimedialog.h"
#include "teamadapter.h"

namespace Ui {
class FightclubClient;
}

class FightclubClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit FightclubClient(QWidget *parent = 0);
    ~FightclubClient();

private:
    Ui::FightclubClient *ui;
    AboutDialog *aboutdlg;
    BroadcastServer *bcastsrv;
    ClockWindow *clockwindow;
    ListAdapter *lstadapt;
    PhasePBar *phpbar;
    ProblemAdapter *probadapt;
    SetTimeDialog *settimedlg;
    TeamAdapter *teamadapt;

    QString previousPath;

    bool continueAndInit();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openClockWindow();
    void clockWindowClosed();

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


public slots:
    void setPhaseProgress(double);

};

#endif // FIGHTCLUBCLIENT_H
