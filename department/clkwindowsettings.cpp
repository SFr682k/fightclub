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


#include <QInputDialog>
#include <QMessageBox>


ClkWindowSettings::ClkWindowSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClkWindowSettings)
{
    ui->setupUi(this);

    ui->chooseFontBox->setEnabled(false);
    ui->chooseScaleBox->setEnabled(false);

    ui->showSecHand->setChecked(true);
    ui->swissRClock->setChecked(true);


    connect(ui->customFontCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomFont(bool)));
    connect(ui->chooseFontBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(selFontChanged(QString)));

    connect(ui->customScaleCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomFontScale(bool)));
    connect(ui->chooseScaleBox, SIGNAL(valueChanged(int)), this, SLOT(scaleFactorChanged(int)));

    connect(ui->showSecHand, SIGNAL(toggled(bool)), this, SLOT(showSecondHand(bool)));

    connect(ui->swissRClock, SIGNAL(clicked(bool)), this, SLOT(setSwissRClock(bool)));
    connect(ui->smoothRClock, SIGNAL(clicked(bool)), this, SLOT(setSmoothRClock(bool)));
    connect(ui->sharpRClock, SIGNAL(clicked(bool)), this, SLOT(setSharpRClock(bool)));


    connect(ui->lockSettingsBttn, SIGNAL(clicked(bool)), this, SLOT(toggleLockedState()));
    connect(ui->closeDialogBttn, SIGNAL(clicked(bool)), this, SLOT(accept()));
}


ClkWindowSettings::~ClkWindowSettings()
{
    delete ui;
}


void ClkWindowSettings::useCustomFont(bool customFont) {
    ui->chooseFontBox->setEnabled(customFont);
    emit fontChanged(customFont? ui->chooseFontBox->currentFont().family() : nullptr);
}

void ClkWindowSettings::selFontChanged(QString fontname) { emit fontChanged(fontname); }



void ClkWindowSettings::useCustomFontScale(bool customScale) {
    ui->chooseScaleBox->setEnabled(customScale);
    emit fontScaleChanged(customScale? ui->chooseScaleBox->value()/100.0 : 1.0);
}

void ClkWindowSettings::scaleFactorChanged(int scale) { emit fontScaleChanged(scale/100.0); }



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


QString ClkWindowSettings::getFontFamily()
    { return (ui->customFontCBox->isChecked()?  ui->chooseFontBox->currentText() : nullptr); }
double  ClkWindowSettings::getFontScale()
    { return (ui->customScaleCBox->isChecked()? ui->chooseScaleBox->value()/100.0 : 1.0); }

int  ClkWindowSettings::getRClkBehavior() {
    if(ui->swissRClock->isChecked())       return 0;
    else if(ui->smoothRClock->isChecked()) return 1;
    else if(ui->sharpRClock->isChecked())  return 2;

    return 0;
}

bool ClkWindowSettings::getShowRClkSecondHand() { return ui->showSecHand->isChecked(); }




void ClkWindowSettings::toggleLockedState() {
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

    ui->appSettings->setEnabled(!locked);
    ui->rclockSettings->setEnabled(!locked);
    ui->lockSettingsBttn->setText(locked? "Unlock" : "Lock");
    ui->lockSettingsBttn->setIcon(locked? QIcon(":/breeze-icons/object-unlocked-16.svg")
                                        : QIcon(":/breeze-icons/object-locked-16.svg"));
}
