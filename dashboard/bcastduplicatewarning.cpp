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


#include "bcastduplicatewarning.h"
#include "ui_bcastduplicatewarning.h"


BcastDuplicateWarning::BcastDuplicateWarning(QString title, uint port, uint id, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BcastDuplicateWarning)
{
    ui->setupUi(this);

    ui->dialogiconlabel->setPixmap(QIcon().fromTheme("dialog-warning").pixmap(64,64));

    ui->titlestring->setText(title);
    ui->portnr->setText(QString::number(port));
    ui->idnr->setText(QString::number(id));

    ui->closeButton->setText("Close [10]");

    closetimer = new QTimer();
    closetimer->start(10000);

    refreshtimer = new QTimer();
    refreshtimer->start(16);

    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateRemainingTime()));

    connect(closetimer, SIGNAL(timeout()), this, SLOT(accept()));
    connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
}


BcastDuplicateWarning::~BcastDuplicateWarning() {
    delete ui;
    delete closetimer;
    delete refreshtimer;
}


void BcastDuplicateWarning::updateRemainingTime() {
    ui->closeButton->setText(QString("Close  [%1]")
                             .arg(QString(closetimer->remainingTime() < 10000? "0" : "")
                                  .append(QString::number(closetimer->remainingTime()/1000))));
}
