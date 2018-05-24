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


#ifndef CLOCKWINDOW_H
#define CLOCKWINDOW_H

#include <QMainWindow>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QString>
#include <QTime>
#include <QTimer>


namespace Ui {
class ClockWindow;
}

class ClockWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit ClockWindow(QWidget *parent = 0);
    ~ClockWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    Ui::ClockWindow *ui;

    QTimer *refreshtimer;
    QString timeToString(int);
    bool roomclock;


    QFont defaultFont;
    double fontScale;

    QTimer *hideCursorTimer;


private slots:
    void updateTime();

    void cursorMoved();
    void hideCursor();

public slots:
    void phaseNameChanged(QString);
    void updateElapsedTime(int);
    void updateMaximumTime(int);
    void problemChanged(QString);
    void performersChanged(QString);
    void toggleRoomclock(bool);

    void setWindowFont(QString);
    void setFontScale(double);

    void showRclockSecondHand(bool);
    void setRclockBehavior(int);
};

#endif // CLOCKWINDOW_H
