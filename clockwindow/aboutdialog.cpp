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


#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->apptitle->setText(QApplication::applicationName());
    ui->versionlabel->setText("Version " + QApplication::applicationVersion());

    QString platformName;
    if(QApplication::platformName() == "xcb")          platformName = "Linux (X11)";
    else if(QApplication::platformName() == "windows") platformName = "Windows";
    else if(QApplication::platformName() == "cocoa")   platformName = "macOS (Cocoa)";
    else                                               platformName = QApplication::platformName();

    ui->qtadvertizing->setText(QString("Using Qt ") + qVersion() + QString(" on ") + platformName);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
