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


#ifndef FIGHTCLUBTELEPRINTER_H
#define FIGHTCLUBTELEPRINTER_H

#include <QMainWindow>

#include "broadcastclient.h"
#include "teleprintersettings.h"

#include <QTimer>


namespace Ui {
class FightclubTeleprinter;
}




class FightclubTeleprinter : public QMainWindow {
    Q_OBJECT

public:
    explicit FightclubTeleprinter(QWidget *parent = 0);
    ~FightclubTeleprinter();

    void setPort(uint);
    void setID(uint);

    void enterFullscreenMode();
    void enterNoConfigMode();

    uint getBcastPort();
    uint getBcastID();

private:
    Ui::FightclubTeleprinter *ui;

    BroadcastClient *bcastcli;
    TeleprinterSettings *settingsdial;

    QString cachedPhaseName, cachedProblem, cachedPerformers;
    int cachedElapsedTime, cachedMaximumTime;
    bool cachedRoomclock;

    bool aboutDialogOpen, settingsDialogOpen;

    QTimer *hideCursorTimer;

protected:
    void mouseMoveEvent(QMouseEvent *event);


signals:
    void toggleRoomclock(bool);
    void updateElapsedTime(int);
    void updateTime();

    void newPort(uint);
    void newID(uint);

    void closeAllClockWindows();

public slots:
    void openAboutDialog();
    void openSettingsDialog();

private slots:
    void cachePhaseName(QString);
    void cacheElapsedTime(int);
    void cacheMaximumTime(int);
    void cacheRoomclock(bool);
    void cacheProblem(QString);
    void cachePerformers(QString);

    void openClockWindow();
    void purgeClockWindows();

    void cursorMoved();
    void hideCursor();
};

#endif // FIGHTCLUBTELEPRINTER_H
