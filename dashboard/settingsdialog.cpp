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


#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);


    ui->dispTournamentNameCBox->setChecked(false);
    ui->tournName->setEnabled(false);

    ui->dispCTimeCBox->setChecked(true);

    ui->customFontCBox->setChecked(false);
    ui->chooseFontBox->setEnabled(false);


    connect(ui->dispTournamentNameCBox, SIGNAL(toggled(bool)), this, SLOT(displayTournamentName(bool)));
    connect(ui->tournName, SIGNAL(textChanged(QString)), this, SLOT(tournamentNameModified(QString)));

    connect(ui->dispCTimeCBox, SIGNAL(toggled(bool)), this, SLOT(displayCurrentTime(bool)));

    connect(ui->customFontCBox, SIGNAL(toggled(bool)), this, SLOT(useCustomFont(bool)));
    connect(ui->chooseFontBox, SIGNAL(activated(QString)), this, SLOT(selFontChanged(QString)));


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
