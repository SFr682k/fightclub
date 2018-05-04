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


#ifndef TELEPRINTERSETTINGS_H
#define TELEPRINTERSETTINGS_H

#include <QDialog>
#include <QKeyEvent>


namespace Ui {
class TeleprinterSettings;
}

class TeleprinterSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TeleprinterSettings(QWidget *parent = 0, unsigned int port = 45454, unsigned int id = 12345);
    ~TeleprinterSettings();
    int getBroadcastPort();
    int getBroadcastID();

private:
    Ui::TeleprinterSettings *ui;
    uint port;
    uint id;
    bool locked;
    QString lockedpwd;
    void setupSelections();

protected:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void fontChanged(QString);
    void fontScaleChanged(double);

    void showRclockSecondHand(bool);
    void rclockBehaviorChanged(int);

public slots:
    void toggleLockedState();
    void setPort(uint);
    void setID(uint);

private slots:
    void enableCustomSettings(bool);
    void applyDefaultSettings(bool);

    void useCustomFont(bool);
    void selFontChanged(QString);
    void useCustomFontScale(bool);
    void scaleFactorChanged(int);

    void showSecondHand(bool);
    void setSwissRClock(bool);
    void setSmoothRClock(bool);
    void setSharpRClock(bool);
};

#endif // TELEPRINTERSETTINGS_H
