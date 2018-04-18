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


#include "setupbroadcastdialog.h"
#include "ui_setupbroadcastdialog.h"

SetupBroadcastDialog::SetupBroadcastDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupBroadcastDialog) {
    ui->setupUi(this);

    connect(ui->defaultSettingsRbttn, SIGNAL(toggled(bool)), this, SLOT(applyDefaultSettings(bool)));
    connect(ui->customSettingsRbttn, SIGNAL(toggled(bool)), this, SLOT(enableCustomSettings(bool)));

    enableCustomSettings(false);
    ui->defaultSettingsRbttn->setChecked(true);
}

SetupBroadcastDialog::~SetupBroadcastDialog() {
    delete ui;
}

int SetupBroadcastDialog::getBroadcastPort() { return ui->selBcastPort->value(); }
int SetupBroadcastDialog::getBroadcastID()   { return ui->selBcastID->value(); }


void SetupBroadcastDialog::applyDefaultSettings(bool apply) {
    if(apply) {
        ui->selBcastPort->setValue(45454);
        ui->selBcastID->setValue(12345);
    }
}

void SetupBroadcastDialog::enableCustomSettings(bool enabled) {
    ui->bcastPortLabel->setEnabled(enabled);
    ui->selBcastPort->setEnabled(enabled);
    ui->bcastIDLabel->setEnabled(enabled);
    ui->selBcastID->setEnabled(enabled);
}
