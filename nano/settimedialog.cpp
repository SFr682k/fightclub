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


#include "settimedialog.h"
#include "ui_settimedialog.h"


SetTimeDialog::SetTimeDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::SetTimeDialog) {
    ui->setupUi(this);

    connect(this, SIGNAL(accepted()), this, SLOT(setTime()));
    connect(ui->etimeselbox, SIGNAL(timeChanged(QTime)), this, SLOT(checkETimeRbttn()));
    connect(ui->rtimeselbox, SIGNAL(timeChanged(QTime)), this, SLOT(checkRTimeRbttn()));
}


SetTimeDialog::~SetTimeDialog() {
    delete ui;
}


void SetTimeDialog::checkETimeRbttn() { ui->etimerbox->setChecked(true); }
void SetTimeDialog::checkRTimeRbttn() { ui->rtimerbox->setChecked(true); }


void SetTimeDialog::setTime() {
    if(ui->etimerbox->isChecked())
        emit elapsedTimeSet(ui->etimeselbox->time().msecsSinceStartOfDay());
    else if(ui->rtimerbox->isChecked())
        emit remainingTimeSet(ui->rtimeselbox->time().msecsSinceStartOfDay());
}

void SetTimeDialog::resetValues() {
    ui->etimeselbox->setTime(QTime(0,0,0,0));
    ui->rtimeselbox->setTime(QTime(0,0,0,0));
}


void SetTimeDialog::setElapsedTime(int elapsedms)
    { ui->etimeselbox->setTime(QTime(0,0,0).addMSecs(elapsedms - (elapsedms % 1000))); }

void SetTimeDialog::setMaximumRTime(int maxrtms)
    { ui->rtimeselbox->setMaximumTime(QTime(0,0,0).addMSecs(maxrtms)); }
