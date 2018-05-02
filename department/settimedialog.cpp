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
    QMessageBox msgbox;
    QTime timeSpec;
    int specMSecs;

    if(ui->etimerbox->isChecked()) {
        timeSpec = ui->etimeselbox->time();
        specMSecs = ((timeSpec.hour()*60 + timeSpec.minute())*60 + timeSpec.second())*1000;
        emit elapsedTimeSet(specMSecs);
    } else if(ui->rtimerbox->isChecked()) {
        timeSpec = ui->rtimeselbox->time();
        specMSecs = ((timeSpec.hour()*60 + timeSpec.minute())*60 + timeSpec.second())*1000;
        emit remainingTimeSet(specMSecs);
    } else if(ui->stimerbox->isChecked()) {
        msgbox.information(0, "Blub", "Not implemented");
    }
}

void SetTimeDialog::resetValues() {
    ui->etimeselbox->setTime(QTime(0,0,0,0));
    ui->rtimeselbox->setTime(QTime(0,0,0,0));
}

void SetTimeDialog::setMaximumRTime(int maxrtms) {
    QTime *maxRTime = new QTime(0,0,0);
    *maxRTime = maxRTime->addMSecs(maxrtms);
    ui->rtimeselbox->setMaximumTime(QTime(maxRTime->hour(), maxRTime->minute(), maxRTime->second()));
}
