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


#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QString>
#include <QTime>
#include <QTimer>

#include "broadcastclient.h"
#include "setupbroadcastdialog.h"

namespace Ui {
class ClockWindow;
}

class ClockWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClockWindow(QWidget *parent = 0);
    ~ClockWindow();
    void setPort(uint);
    void setID(uint);
    void openAboutDialog();
    void openSetupBCastDialog();
    uint getBcastPort();
    uint getBcastID();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::ClockWindow *ui;
    BroadcastClient *bcastcli;
    SetupBroadcastDialog *setupbcastdial;
    bool aboutDialogOpen;
    bool bcastSettingsOpen;
    QTimer *refreshtimer;
    QString timeToString(int);
    bool roomclock;
    uint bcastPort, bcastID;

signals:
    void newPort(uint);
    void newID(uint);

private slots:
    void toggleRoomclock(bool);
    void updateElapsedTime(int);
    void updateTime();
};

#endif // CLOCKWINDOW_H
