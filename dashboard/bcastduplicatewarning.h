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


#ifndef BCASTDUPLICATEWARNING_H
#define BCASTDUPLICATEWARNING_H

#include <QDialog>

#include <QTimer>

namespace Ui {
class BcastDuplicateWarning;
}

class BcastDuplicateWarning : public QDialog
{
    Q_OBJECT

public:
    explicit BcastDuplicateWarning(QString, uint, uint, QWidget *parent = 0);
    ~BcastDuplicateWarning();

private:
    Ui::BcastDuplicateWarning *ui;

    QTimer *closetimer, *refreshtimer;

private slots:
    void updateRemainingTime();
};

#endif // BCASTDUPLICATEWARNING_H
