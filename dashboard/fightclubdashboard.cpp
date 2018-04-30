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


#include "fightclubdashboard.h"
#include "ui_fightclubdashboard.h"

#include "aboutdialog.h"

#include <QTime>


FightclubDashboard::FightclubDashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FightclubDashboard)
{
    ui->setupUi(this);

    refreshtimer = new QTimer();
    refreshtimer->start(30);

    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateTimeDisplay()));

    aboutDialogOpen = false;
}


FightclubDashboard::~FightclubDashboard() {
    delete ui;
}


void FightclubDashboard::openAboutDialog() {
    if(!aboutDialogOpen) {
        aboutDialogOpen = true;
        AboutDialog *ad = new AboutDialog(this);
        ad->exec();
        aboutDialogOpen = false;
    }
}




void FightclubDashboard::updateTimeDisplay() {
    QString currentTime = QTime::currentTime().toString("HH:mm");
    if(QTime::currentTime().second() % 2 != 0) currentTime[2] = ' ';
    ui->currenttime->display(currentTime);
}



void FightclubDashboard::resizeEvent(QResizeEvent *event) {
    QFont tnamefont = font();
    tnamefont.setBold(true);
    tnamefont.setPointSize(height()*0.026);
    ui->tournamentname->setFont(tnamefont);

    /*
    QFont phaselabelfont = ui->phaselabel->font();
    phaselabelfont.setPointSize(2 + height()*0.03);

    QFont infolabelfont = ui->perflabel->font();
    infolabelfont.setPointSize(4 + height()*0.015);

    ui->phaselabel->setFont(phaselabelfont);
    ui->problabel->setFont(infolabelfont);
    ui->perflabel->setFont(infolabelfont);
    */

    QWidget::resizeEvent(event);
}



void FightclubDashboard::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_F1:
            openAboutDialog();
            break;

        case Qt::Key_F:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                setWindowState(Qt::WindowFullScreen);
            break;

        case Qt::Key_Escape:
            if(windowState() == Qt::WindowFullScreen) setWindowState(Qt::WindowMaximized);
            break;

        case Qt::Key_Q:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                this->close();
            break;

        default:
            QWidget::keyPressEvent(event);
    }
}
