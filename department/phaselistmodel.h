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


#ifndef PHASEMODEL_H
#define PHASEMODEL_H


#include <QAbstractTableModel>
#include <QColor>
#include <QTime>


class Phase {
public:
    Phase();
    Phase(QTime duration, QTime overtime, QString name,
          bool repperform = false, bool oppperform = false, bool revperform = false,
          bool autoadvance = false, bool carry = false, bool ocarry = false, bool roomclock = false);
    QTime getDuration();
    QTime getOvertime();
    QString getName();
    bool getAutoadvance();
    bool getCarry();
    bool getOCarry();
    bool getRoomclock();
    bool getRepPerform();
    bool getOppPerform();
    bool getRevPerform();

private:
    QTime duration;
    QTime overtime;
    QString name;
    bool autoadvance;
    bool carry;
    bool ocarry;
    bool roomclock;
    bool repperform;
    bool oppperform;
    bool revperform;
};



class PhaseListModel : public QAbstractTableModel {
    Q_OBJECT

public:
    PhaseListModel(QObject *parent=0);
    PhaseListModel(QList<Phase>, QObject *parent=0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setHighlightedRow(int);
    QList<Phase> getPhasesList();


private:
    QList<Phase> listofphases;
    int highlightedRow;
    QColor highlightColor;
};

#endif // PHASEMODEL_H
