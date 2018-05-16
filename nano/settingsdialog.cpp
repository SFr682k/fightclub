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

    fcNano = parent;
    locked = false;

    previousPath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation).value(0);

    ui->customFontCBox->setChecked(false);
    ui->chooseFontBox->setEnabled(false);
    ui->chooseScaleBox->setEnabled(false);
    ui->chooseBttnScaleBox->setEnabled(false);

    ui->phasesListDescr->setText(" ");
    ui->unloadPhasesList->setEnabled(false);


    connect(ui->customFontCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomFont(bool)));
    connect(ui->chooseFontBox, SIGNAL(activated(QString)), this, SLOT(selFontChanged(QString)));

    connect(ui->customScaleCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomFontScale(bool)));
    connect(ui->chooseScaleBox, SIGNAL(valueChanged(int)), this, SLOT(scaleFactorChanged(int)));

    connect(ui->customBttnScaleCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomButtonScale(bool)));
    connect(ui->chooseBttnScaleBox, SIGNAL(valueChanged(int)), this, SLOT(buttonScaleChanged(int)));


    connect(ui->loadPhasesList, SIGNAL(clicked(bool)), this, SLOT(loadPhasesList()));
    connect(ui->unloadPhasesList, SIGNAL(clicked(bool)), this, SLOT(unloadPhasesList()));


    connect(ui->lockSettings, SIGNAL(clicked(bool)), this, SLOT(toggleLockedState()));
    connect(ui->closeDialogBttn, SIGNAL(clicked(bool)), this, SLOT(accept()));
}


SettingsDialog::~SettingsDialog() {
    delete ui;
}


int SettingsDialog::exec() {
    if(locked && (lockedpwd == nullptr)) {
        QMessageBox::critical(fcNano, "Nice try …",
                              "Configuration of Fightclub Department is (currently) disabled.");
        return 0;
    } else return QDialog::exec();
}



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

void SettingsDialog::useCustomButtonScale(bool customScale) {
    ui->chooseBttnScaleBox->setEnabled(customScale);
    emit buttonScaleChanged(customScale? ui->chooseScaleBox->value()/100.0 : 1.0);
}

void SettingsDialog::buttonScaleChanged(int scale) { emit buttonScaleChanged(scale/100.0); }



void SettingsDialog::loadPhasesList() {
    QFileDialog *selectPhasesFileDialog
                = new QFileDialog(this, "Select a supported phases file", previousPath,
                                  "Supported Fightclub phases files (*.fcnano *.fcphases)");

    if(selectPhasesFileDialog->exec()) {
        QString file = selectPhasesFileDialog->selectedFiles().value(0);

        loadPhasesList(file);
        previousPath = selectPhasesFileDialog->directory().absolutePath();
    }
}


void SettingsDialog::loadPhasesList(QString file) {
    FilePropertyParser *fpp = new FilePropertyParser(file);

    std::string stdstrpath = file.toStdString();
    unsigned lastdot = stdstrpath.find_last_of(".");
    QString fileext = QString::fromStdString(stdstrpath.substr(lastdot + 1, stdstrpath.length()));

    if(!((fpp->getFileType() == nullptr)
         || fpp->getFileType().contains("nano", Qt::CaseInsensitive)
         || fpp->getFileType().contains("phases", Qt::CaseInsensitive))) {

        QMessageBox::critical(this, "Unsupported file format",
                              "Fightclub Nano doesn't support " + fpp->getFileType() + " files.");
        return;
    }


    if(fpp->getFileType().contains("nano", Qt::CaseInsensitive) && !(fileext.contains("fcphases", Qt::CaseInsensitive)))
        emit loadListOfNanoPhases(file);
    else if(fpp->getFileType().contains("phases", Qt::CaseInsensitive) && !(fileext.contains("fcnano", Qt::CaseInsensitive)))
        emit loadListOfPhases(file);
    else {
        int clickedBttn =  QMessageBox::critical(this, "Well, that’s strange …",
                                                 "You’ve loaded a " + fpp->getFileType() + " file, but the file extension is “" + fileext + "”.",
                                                 "Trust the file header", "Trust the file extension", "Don’t load this file", 2);
        if(clickedBttn == 2) return;

        if(clickedBttn == 1) {
            if(fileext.contains("fcnano", Qt::CaseInsensitive))
                emit loadListOfNanoPhases(file);
            else if(fileext.contains("fcphases", Qt::CaseInsensitive))
                emit loadListOfPhases(file);
        } else if(clickedBttn == 0) {
            if(fpp->getFileType().contains("nano", Qt::CaseInsensitive))
                emit loadListOfNanoPhases(file);
            else if(fpp->getFileType().contains("phases", Qt::CaseInsensitive))
                emit loadListOfPhases(file);
        }
    }


    ui->phasesListTitle->setText(fpp->getTitle());
    ui->phasesListDescr->setText(fpp->getDescription());

    ui->unloadPhasesList->setEnabled(true);
}


void SettingsDialog::unloadPhasesList() {
    emit unloadListOfPhases();
    ui->unloadPhasesList->setEnabled(false);
    ui->phasesListTitle->setText("No list of phases loaded");
    ui->phasesListDescr->setText(" ");
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


void SettingsDialog::enterNoConfigMode() {
    ui->configTabs->setEnabled(false);
    locked = true;
    lockedpwd = nullptr;
    ui->lockSettings->setText("Locked");
    ui->lockSettings->setIcon(QIcon(":/breeze-icons/object-locked-16.svg"));
    ui->lockSettings->setEnabled(false);
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
