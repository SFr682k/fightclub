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


#ifndef SETUPBROADCASTDIALOG_H
#define SETUPBROADCASTDIALOG_H

#include <QDialog>

namespace Ui {
class SetupBroadcastDialog;
}

class SetupBroadcastDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupBroadcastDialog(QWidget *parent = 0);
    ~SetupBroadcastDialog();
    int getBroadcastPort();
    int getBroadcastID();

private slots:
    void enableCustomSettings(bool);
    void applyDefaultSettings(bool);

private:
    Ui::SetupBroadcastDialog *ui;
};

#endif // SETUPBROADCASTDIALOG_H
