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


#ifndef FIGHTCLUBNANO_H
#define FIGHTCLUBNANO_H

#include "clocklogic.h"
#include "listadapter.h"
#include "settimedialog.h"
#include "settingsdialog.h"

#include <QMainWindow>

#include <QKeyEvent>
#include <QTimer>

namespace Ui {
class FightclubNano;
}

class FightclubNano : public QMainWindow
{
    Q_OBJECT

public:
    explicit FightclubNano(QWidget *parent = 0);
    ~FightclubNano();

    void openAboutDialog();
    void openSettingsDialog();

private:
    Ui::FightclubNano *ui;

    ClockLogic *clklgk;
    ListAdapter *lstadapt;
    SetTimeDialog *settimedlg;
    SettingsDialog *settingsdlg;

    bool roomclock;

    QTimer *refreshtimer;
    bool aboutDialogOpen, settingsDialogOpen;

    QFont defaultFont;
    double fontScale, buttonScale;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void init();

    void toggleStartStopBttn();
    void openSetTimeDialog();
    void updateLCDDisplay();
    void setRoomclock(bool);

    void setApplicationFont(QString);
    void setFontScale(double);
    void setButtonScale(double);
};

#endif // FIGHTCLUBNANO_H
