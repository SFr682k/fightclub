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


#include "phaselistmodel.h"



Phase::Phase() { }

Phase::Phase(QTime d, QTime o, QString n, bool a, bool c, bool oc, bool r) {
    duration = d;
    overtime = o;
    name = n;
    autoadvance = a;
    carry = c;
    ocarry = oc;
    roomclock = r;
}

QTime Phase::getDuration()   { return duration; }
QTime Phase::getOvertime()   { return overtime; }
QString Phase::getName()     { return name; }
bool Phase::getAutoadvance() { return autoadvance; }
bool Phase::getCarry()       { return carry; }
bool Phase::getOCarry()      { return ocarry; }
bool Phase::getRoomclock()   { return roomclock; }



PhaseListModel::PhaseListModel(QObject *parent) : QAbstractTableModel(parent)  {
    highlightColor = QColor::fromRgb(0xff, 0x99, 0x11);
    highlightedRow = -1;
}

PhaseListModel::PhaseListModel(QList<Phase> phases, QObject *parent) : QAbstractTableModel(parent) {
    highlightColor = QColor::fromRgb(0xff, 0x99, 0x11);
    listofphases = phases;
    highlightedRow = -1;
}


int PhaseListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return listofphases.size();
}

int PhaseListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 7;
}

QVariant PhaseListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= listofphases.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        Phase phase = listofphases.at(index.row());

        if(index.column() == 0)      return phase.getName();
        else if(index.column() == 1) return phase.getDuration().toString("H:mm:ss");
        else if(index.column() == 2) return phase.getOvertime().toString("H:mm:ss");
        else if(index.column() == 3) return phase.getAutoadvance();
        else if(index.column() == 4) return phase.getCarry();
        else if(index.column() == 5) return phase.getOCarry();
        else if(index.column() == 6) return phase.getRoomclock();
    }

    if (role == Qt::BackgroundRole) {
        if(index.row() == highlightedRow)
            return highlightColor;
    }

    return QVariant();
}






void PhaseListModel::setHighlightedRow(int row){
    if (row < listofphases.size() && row >= -1){
        int tlRow = (row < highlightedRow) ? row : highlightedRow;
        int brRow = (row < highlightedRow) ? highlightedRow : row;
        highlightedRow = row;
        emit dataChanged(this->index(tlRow,0),this->index(brRow,0));
    }
}



QList<Phase> PhaseListModel::getPhasesList() {
    return listofphases;
}
