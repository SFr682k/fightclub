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


#ifndef FIGHTCLUBDASHBOARD_H
#define FIGHTCLUBDASHBOARD_H

#include <QMainWindow>

#include "multibroadcastclient.h"
#include "settingsdialog.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QKeyEvent>
#include <QLabel>
#include <QStackedWidget>
#include <QTimer>


namespace Ui {
class FightclubDashboard;
}

class FightclubDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit FightclubDashboard(QWidget *parent = 0);
    ~FightclubDashboard();
    void openAboutDialog();
    void openSettingsDialog();

private:
    Ui::FightclubDashboard *ui;
    SettingsDialog *settingsdial;

    QFont defaultFont;
    bool displayCurrTime;

    MultiBroadcastClient *mbcastclient;
    bool aboutDialogOpen;
    int numberOfDepartments;
    QTimer *refreshtimer, *switchpagetimer;

    QStackedWidget *container;
    QGridLayout *currentGrid;


protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;


signals:
    void screenSizeChanged(int);


private slots:
    void addDepartmentBox(SignalHelper*);
    void removeAllDepartmentBoxes();
    void nextContainerPage();
    void updateTimeDisplay();
    void setDisplayCurrTime(bool);
    void setApplicationFont(QString);
};

#endif // FIGHTCLUBDASHBOARD_H
