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


#ifndef SETTIMEDIALOG_H
#define SETTIMEDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class SetTimeDialog;
}

class SetTimeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetTimeDialog(QWidget *parent = 0);
    void resetValues();
    void setElapsedTime(int);
    void setMaximumRTime(int);
    void setSavedTime(int);
    ~SetTimeDialog();

signals:
    void elapsedTimeSet(int);
    void remainingTimeSet(int);

private slots:
    void setTime();
    void checkETimeRbttn();
    void checkRTimeRbttn();
    void checkSTimeRbttn();

private:
    Ui::SetTimeDialog *ui;
};

#endif // SETTIMEDIALOG_H
