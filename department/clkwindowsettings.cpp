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


#include "clkwindowsettings.h"
#include "ui_clkwindowsettings.h"


ClkWindowSettings::ClkWindowSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClkWindowSettings)
{
    ui->setupUi(this);

    ui->showSecHand->setChecked(true);
    ui->swissRClock->setChecked(true);

    connect(ui->showSecHand, SIGNAL(toggled(bool)), this, SLOT(showSecondHand(bool)));

    connect(ui->swissRClock, SIGNAL(clicked(bool)), this, SLOT(setSwissRClock(bool)));
    connect(ui->smoothRClock, SIGNAL(clicked(bool)), this, SLOT(setSmoothRClock(bool)));
    connect(ui->sharpRClock, SIGNAL(clicked(bool)), this, SLOT(setSharpRClock(bool)));
}


ClkWindowSettings::~ClkWindowSettings()
{
    delete ui;
}



void ClkWindowSettings::showSecondHand(bool show) {
    ui->sechandmvmtCaption->setEnabled(show);
    ui->smoothRClock->setEnabled(show);
    ui->sharpRClock->setEnabled(show);
    ui->swissRClock->setEnabled(show);
    emit showRclockSecondHand(show);
}


void ClkWindowSettings::setSwissRClock(bool set)  { if(set) emit rclockBehaviorChanged(0); }
void ClkWindowSettings::setSmoothRClock(bool set) { if(set) emit rclockBehaviorChanged(1); }
void ClkWindowSettings::setSharpRClock(bool set)  { if(set) emit rclockBehaviorChanged(2); }

