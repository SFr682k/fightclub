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


#include "teamitemlistmodel.h"


Team::Team(QString id, QString name, QList<QString> members) {
    teamid = id;
    teamname = name;
    teammembers = members;
}

QString Team::getTeamname() { return teamname; }




TeamItemListModel::TeamItemListModel(QObject *parent) : QAbstractTableModel(parent) {}

TeamItemListModel::TeamItemListModel(QList<Team> teams, QObject *parent) : QAbstractTableModel(parent)  {
    listofteams = teams;
}


int TeamItemListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return listofteams.length();
}


int TeamItemListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}


QVariant TeamItemListModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) return QVariant();
    if(index.row() >= listofteams.size() || index.row() < 0) return QVariant();

    if(role == Qt::DisplayRole) {
        Team team = listofteams.at(index.row());

        return team.getTeamname();
    }

    return QVariant();
}
