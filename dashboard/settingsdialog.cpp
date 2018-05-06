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


#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "filepropertyparser.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QStandardPaths>



SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    previousPath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0);

    ui->dispTournamentNameCBox->setChecked(false);
    ui->tournName->setEnabled(false);

    ui->dispCTimeCBox->setChecked(true);

    ui->customFontCBox->setChecked(false);
    ui->chooseFontBox->setEnabled(false);
    ui->chooseScaleBox->setEnabled(false);

    ui->departmentListDescr->setText(" ");
    ui->unloadDepartmentList->setEnabled(false);


    connect(ui->dispTournamentNameCBox, SIGNAL(toggled(bool)), this, SLOT(displayTournamentName(bool)));
    connect(ui->tournName, SIGNAL(textChanged(QString)), this, SLOT(tournamentNameModified(QString)));

    connect(ui->dispCTimeCBox, SIGNAL(toggled(bool)), this, SLOT(displayCurrentTime(bool)));

    connect(ui->customFontCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomFont(bool)));
    connect(ui->chooseFontBox, SIGNAL(activated(QString)), this, SLOT(selFontChanged(QString)));

    connect(ui->customScaleCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomFontScale(bool)));
    connect(ui->chooseScaleBox, SIGNAL(valueChanged(int)), this, SLOT(scaleFactorChanged(int)));


    connect(ui->loadDepartmentList, SIGNAL(clicked(bool)), this, SLOT(loadDepList()));
    connect(ui->unloadDepartmentList, SIGNAL(clicked(bool)), this, SLOT(unloadDepList()));


    connect(ui->lockSettings, SIGNAL(clicked(bool)), this, SLOT(toggleLockedState()));
    connect(ui->closeDialogBttn, SIGNAL(clicked(bool)), this, SLOT(accept()));
}


SettingsDialog::~SettingsDialog() {
    delete ui;
}



void SettingsDialog::displayTournamentName(bool display) {
    ui->tournName->setEnabled(display);
    if(!display) ui->tournName->clear();
}


void SettingsDialog::tournamentNameModified(QString tname) {
    if(tname == "") emit tournamentNameChanged(" ");
    else            emit tournamentNameChanged(tname);
}

void SettingsDialog::displayCurrentTime(bool display) { emit displayCTimeChanged(display); }


void SettingsDialog::useCustomFont(bool customFont) {
    ui->chooseFontBox->setEnabled(customFont);
    emit fontChanged(customFont? ui->chooseFontBox->currentFont().family() : nullptr);
}

void SettingsDialog::selFontChanged(QString fontname) { emit fontChanged(fontname); }



void SettingsDialog::useCustomFontScale(bool customScale) {
    ui->chooseScaleBox->setEnabled(customScale);
    emit fontScaleChanged(customScale? ui->chooseScaleBox->value()/100.0 : 1.0);
}

void SettingsDialog::scaleFactorChanged(int scale) { emit fontScaleChanged(scale/100.0); }



void SettingsDialog::loadDepList() {
    QFileDialog *selectDepFileDialog
                = new QFileDialog(this, "Select a departments file", previousPath, "Fightclub departments files (*.fcdeps)");

    if(selectDepFileDialog->exec()) {
        QString file = selectDepFileDialog->selectedFiles().value(0);

        FilePropertyParser *fpp = new FilePropertyParser(file);

        if(!(fpp->getFileType() == nullptr || fpp->getFileType().contains("departments", Qt::CaseInsensitive))) {
            QMessageBox::critical(this, "Wrong file format",
                                  "You requested a departments file, but " + QFileInfo(file).fileName() + " is a " + fpp->getFileType() + " file.");
            return;
        }

        emit loadListOfDepartments(file);
        previousPath = selectDepFileDialog->directory().absolutePath();


        ui->departmentListTitle->setText(fpp->getTitle());
        ui->departmentListDescr->setText(fpp->getDescription());
        ui->unloadDepartmentList->setEnabled(true);
    }
}


void SettingsDialog::unloadDepList() {
    emit unloadListOfDepartments();
    ui->unloadDepartmentList->setEnabled(false);
    ui->departmentListTitle->setText("No list of departments loaded");
    ui->departmentListDescr->setText(" ");
}




void SettingsDialog::toggleLockedState() {
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

    ui->configTabs->setEnabled(!locked);

    ui->lockSettings->setText(locked? "Unlock" : "Lock");
    ui->lockSettings->setIcon(locked? QIcon(":/breeze-icons/object-unlocked-16.svg")
                                    : QIcon(":/breeze-icons/object-locked-16.svg"));
}


void SettingsDialog::keyPressEvent(QKeyEvent *event) {
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
