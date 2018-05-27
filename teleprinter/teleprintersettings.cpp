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


#include "teleprintersettings.h"
#include "ui_teleprintersettings.h"

#include <QInputDialog>
#include <QMessageBox>

TeleprinterSettings::TeleprinterSettings(QWidget *parent, unsigned int bcastport, unsigned int bcastid) :
    QDialog(parent),
    ui(new Ui::TeleprinterSettings) {
    ui->setupUi(this);

    fcTeleprinter = parent;

    port = bcastport;
    id = bcastid;
    setupSelections();

    locked = false;


    ui->chooseFontBox->setEnabled(false);
    ui->chooseScaleBox->setEnabled(false);

    ui->showSecHand->setChecked(true);
    ui->swissRClock->setChecked(true);


    connect(ui->defaultSettingsRbttn, SIGNAL(toggled(bool)), this, SLOT(applyDefaultSettings(bool)));
    connect(ui->customSettingsRbttn, SIGNAL(toggled(bool)), this, SLOT(enableCustomSettings(bool)));

    connect(ui->applyBroadcastSettings, SIGNAL(clicked(bool)), this, SLOT(applyBroadcastSettings()));


    connect(ui->customFontCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomFont(bool)));
    connect(ui->chooseFontBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(selFontChanged(QString)));

    connect(ui->customScaleCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomFontScale(bool)));
    connect(ui->chooseScaleBox, SIGNAL(valueChanged(int)), this, SLOT(scaleFactorChanged(int)));

    connect(ui->showSecHand, SIGNAL(toggled(bool)), this, SLOT(showSecondHand(bool)));

    connect(ui->swissRClock, SIGNAL(clicked(bool)), this, SLOT(setSwissRClock(bool)));
    connect(ui->smoothRClock, SIGNAL(clicked(bool)), this, SLOT(setSmoothRClock(bool)));
    connect(ui->sharpRClock, SIGNAL(clicked(bool)), this, SLOT(setSharpRClock(bool)));


    connect(ui->lockSettings, SIGNAL(clicked(bool)), this, SLOT(toggleLockedState()));
    connect(ui->closeDialogBttn, SIGNAL(clicked(bool)), this, SLOT(accept()));


    setupSelections();
}

TeleprinterSettings::~TeleprinterSettings() {
    delete ui;
}


int TeleprinterSettings::exec() {
    if(locked && (lockedpwd == nullptr)) {
        QMessageBox::critical(fcTeleprinter, "Nice try …",
                              "Configuration of Fightclub Teleprinter is (currently) disabled.");
        return 0;
    } else return QDialog::exec();
}







// BROADCAST SETTINGS -------------------------------------------------------------------

void TeleprinterSettings::setupSelections() {
    if((port != 45454) || (id != 12345)) {
        ui->customSettingsRbttn->setChecked(true);
        ui->selBcastPort->setValue(port);
        ui->selBcastID->setValue(id);
    } else {
        ui->defaultSettingsRbttn->setChecked(true);
        enableCustomSettings(false);
    }
}


void TeleprinterSettings::setPort(uint newport) {
    if(newport > 0) port = newport%65536;
    else            port = 45454;
    setupSelections();
}

void TeleprinterSettings::setID(uint newid) {
    id = newid;
    setupSelections();
}


void TeleprinterSettings::applyDefaultSettings(bool apply) {
    if(apply) {
        ui->selBcastPort->setValue(45454);
        ui->selBcastID->setValue(12345);
    }
}

void TeleprinterSettings::enableCustomSettings(bool enabled) {
    ui->bcastPortLabel->setEnabled(enabled);
    ui->selBcastPort->setEnabled(enabled);
    ui->bcastIDLabel->setEnabled(enabled);
    ui->selBcastID->setEnabled(enabled);
}

void TeleprinterSettings::applyBroadcastSettings()
    { emit bcastAddressChanged(ui->selBcastPort->value(), ui->selBcastID->value()); }







// APPEARANCE SETTINGS ------------------------------------------------------------------

void TeleprinterSettings::useCustomFont(bool customFont) {
    ui->chooseFontBox->setEnabled(customFont);
    emit fontChanged(customFont? ui->chooseFontBox->currentFont().family() : nullptr);
}

void TeleprinterSettings::selFontChanged(QString fontname) { emit fontChanged(fontname); }



void TeleprinterSettings::useCustomFontScale(bool customScale) {
    ui->chooseScaleBox->setEnabled(customScale);
    emit fontScaleChanged(customScale? ui->chooseScaleBox->value()/100.0 : 1.0);
}

void TeleprinterSettings::scaleFactorChanged(int scale) { emit fontScaleChanged(scale/100.0); }







// ROOMCLOCK SETTINGS -------------------------------------------------------------------

void TeleprinterSettings::showSecondHand(bool show) {
    ui->sechandmvmtCaption->setEnabled(show);
    ui->smoothRClock->setEnabled(show);
    ui->sharpRClock->setEnabled(show);
    ui->swissRClock->setEnabled(show);
    emit showRclockSecondHand(show);
}


void TeleprinterSettings::setSwissRClock(bool set)  { if(set) emit rclockBehaviorChanged(0); }
void TeleprinterSettings::setSmoothRClock(bool set) { if(set) emit rclockBehaviorChanged(1); }
void TeleprinterSettings::setSharpRClock(bool set)  { if(set) emit rclockBehaviorChanged(2); }







// METHODS FOR GETTING STUFF ------------------------------------------------------------

QString TeleprinterSettings::getFontFamily()
    { return (ui->customFontCBox->isChecked()?  ui->chooseFontBox->currentText() : nullptr); }
double  TeleprinterSettings::getFontScale()
    { return (ui->customScaleCBox->isChecked()? ui->chooseScaleBox->value()/100.0 : 1.0); }

int  TeleprinterSettings::getRClkBehavior() {
    if(ui->swissRClock->isChecked())       return 0;
    else if(ui->smoothRClock->isChecked()) return 1;
    else if(ui->sharpRClock->isChecked())  return 2;

    return 0;
}

bool TeleprinterSettings::getShowRClkSecondHand() { return ui->showSecHand->isChecked(); }







// INTERNAL STUFF -----------------------------------------------------------------------

void TeleprinterSettings::toggleLockedState() {
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

    ui->settingsTabs->setEnabled(!locked);
    ui->lockSettings->setText(locked? "Unlock" : "Lock");
    ui->lockSettings->setIcon(locked? QIcon(":/breeze-icons/object-unlocked-16.svg")
                                    : QIcon(":/breeze-icons/object-locked-16.svg"));
}



void TeleprinterSettings::enterNoConfigMode() {
    ui->settingsTabs->setEnabled(false);
    locked = true;
    lockedpwd = nullptr;
    ui->lockSettings->setText("Locked");
    ui->lockSettings->setIcon(QIcon(":/breeze-icons/object-locked-16.svg"));
    ui->lockSettings->setEnabled(false);
}


void TeleprinterSettings::keyPressEvent(QKeyEvent *event) {
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
