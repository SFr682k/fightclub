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


#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    int exec();

    void loadDepList(QString);
    void enterNoConfigMode();


private:
    QWidget *fcDashboard;
    Ui::SettingsDialog *ui;

    QString previousPath;

    bool locked;
    QString lockedpwd;


protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void tournamentNameChanged(QString);
    void displayCTimeChanged(bool);
    void fontChanged(QString);
    void fontScaleChanged(double);

    void loadListOfDepartments(QString);
    void unloadListOfDepartments();

private slots:
    void displayTournamentName(bool);
    void tournamentNameModified(QString);
    void displayCurrentTime(bool);
    void useCustomFont(bool);
    void selFontChanged(QString);
    void useCustomFontScale(bool);
    void scaleFactorChanged(int);

    void loadDepList();
    void unloadDepList();

    void toggleLockedState();
};

#endif // SETTINGSDIALOG_H
