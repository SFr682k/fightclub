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


#ifndef TELEPRINTER_H
#define TELEPRINTER_H

#include <QMainWindow>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QString>
#include <QTime>
#include <QTimer>

#include "broadcastclient.h"
#include "teleprintersettings.h"

namespace Ui {
class FightclubTeleprinter;
}

class FightclubTeleprinter : public QMainWindow
{
    Q_OBJECT

public:
    explicit FightclubTeleprinter(QWidget *parent = 0);
    ~FightclubTeleprinter();
    void setPort(uint);
    void setID(uint);
    void openAboutDialog();
    void openSettingsDialog();
    uint getBcastPort();
    uint getBcastID();

    void enterFullscreenMode();
    void enterNoConfigMode();


protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::FightclubTeleprinter *ui;
    BroadcastClient *bcastcli;
    TeleprinterSettings *settingsdial;

    bool aboutDialogOpen, settingsDialogOpen;

    QFont defaultFont;
    double fontScale;

    QTimer *refreshtimer;
    QString timeToString(int);
    bool roomclock;

    uint bcastPort, bcastID;

    QTimer *hideCursorTimer;


signals:
    void newPort(uint);
    void newID(uint);

private slots:
    void toggleRoomclock(bool);
    void updateElapsedTime(int);
    void updateTime();

    void setWindowFont(QString);
    void setFontScale(double);

    void cursorMoved();
    void hideCursor();
};

#endif // TELEPRINTER_H
