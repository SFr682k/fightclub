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

#include <QInputDialog>
#include <QMessageBox>

SetupBroadcastDialog::SetupBroadcastDialog(QWidget *parent, unsigned int bcastport, unsigned int bcastid) :
    QDialog(parent),
    ui(new Ui::SetupBroadcastDialog) {
    ui->setupUi(this);

    port = bcastport;
    id = bcastid;
    setupSelections();

    locked = false;


    connect(ui->defaultSettingsRbttn, SIGNAL(toggled(bool)), this, SLOT(applyDefaultSettings(bool)));
    connect(ui->customSettingsRbttn, SIGNAL(toggled(bool)), this, SLOT(enableCustomSettings(bool)));

    connect(ui->lockSettings, SIGNAL(clicked(bool)), this, SLOT(toggleLockedState()));
    connect(ui->applySettings, SIGNAL(clicked(bool)), this, SLOT(accept()));


    setupSelections();
}

SetupBroadcastDialog::~SetupBroadcastDialog() {
    delete ui;
}


void SetupBroadcastDialog::setupSelections() {
    if((port != 45454) || (id != 12345)) {
        ui->customSettingsRbttn->setChecked(true);
        ui->selBcastPort->setValue(port);
        ui->selBcastID->setValue(id);
    } else {
        ui->defaultSettingsRbttn->setChecked(true);
        enableCustomSettings(false);
    }
}

int SetupBroadcastDialog::getBroadcastPort() { return ui->selBcastPort->value(); }
int SetupBroadcastDialog::getBroadcastID()   { return ui->selBcastID->value(); }


void SetupBroadcastDialog::setPort(uint newport) {
    if(newport > 0) port = newport%65536;
    else            port = 45454;
    setupSelections();
}

void SetupBroadcastDialog::setID(uint newid) {
    id = newid;
    setupSelections();
}


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


void SetupBroadcastDialog::toggleLockedState() {
    if(!locked) {
        QString pwd1 = QInputDialog::getText(this, "Set password",
                                             "Please set a password:", QLineEdit::Password);

        if(pwd1 == nullptr) return;

        QString pwd2 = QInputDialog::getText(this, "Set password",
                                             "Please confirm the password:", QLineEdit::Password);

        if(pwd2 == nullptr) return;
        else if(pwd1 != pwd2) {
            QMessageBox::critical(this, "Error", "The passwords are not identical");
            return;
        }

        locked = true;
        lockedpwd = pwd1;
    } else {
        QString pwd = QInputDialog::getText(this, "Enter password",
                                            "Please enter the password:", QLineEdit::Password);

        if(pwd == nullptr) return;
        else if(pwd != lockedpwd) {
            QMessageBox::critical(this, "Error", "Wrong password");
            return;
        }

        locked = false;
    }

    ui->bcastSettings->setEnabled(!locked);
    ui->lockSettings->setText(locked? "Unlock" : "Lock");
    ui->lockSettings->setIcon(locked? QIcon(":/breeze-icons/object-unlocked-16.png")
                                    : QIcon(":/breeze-icons/object-locked-16.png"));
}




void SetupBroadcastDialog::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_Enter:
            this->accept();
            break;

        case Qt::Key_Escape:
            this->reject();
            break;

        default:
            QWidget::keyPressEvent(event);
    }
}
