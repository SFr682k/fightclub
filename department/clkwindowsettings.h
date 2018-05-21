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


#ifndef CLKWINDOWSETTINGS_H
#define CLKWINDOWSETTINGS_H

#include <QDialog>

namespace Ui {
class ClkWindowSettings;
}

class ClkWindowSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ClkWindowSettings(QWidget *parent = 0);
    ~ClkWindowSettings();

    QString getFontFamily();
    double  getFontScale();
    int     getRClkBehavior();
    bool    getShowRClkSecondHand();

private:
    Ui::ClkWindowSettings *ui;

    bool locked;
    QString lockedpwd;

signals:
    void fontChanged(QString);
    void fontScaleChanged(double);
    void showRclockSecondHand(bool);
    void rclockBehaviorChanged(int);

private slots:
    void useCustomFont(bool);
    void selFontChanged(QString);

    void useCustomFontScale(bool);
    void scaleFactorChanged(int);

    void showSecondHand(bool);
    void setSmoothRClock(bool);
    void setSharpRClock(bool);
    void setSwissRClock(bool);

    void toggleLockedState();
};

#endif // CLKWINDOWSETTINGS_H
