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


#include "teamadapter.h"

#include <QFile>
#include <QTextStream>


#include <QDebug>


TeamAdapter::TeamAdapter(QObject *parent) : QObject(parent) {
    teamlist = new TeamItemListModel();
}



int TeamAdapter::loadTeamsFromFile(QString path) {
    QList<Team> tmplist;
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 1;

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList splitline = line.split('\t');


        if(splitline.size() > 1) {
            // We're handling an team with an ID and a name

            // Get all team members and add them to a list
            QList<QString> members;
            for(int i = 2; i < splitline.size(); i++)
                members.append(splitline.value(i));

            tmplist.append(Team(splitline.value(0), splitline.value(1), members));
        }

    }

    teamlist = new TeamItemListModel(tmplist);
    file.close();

    return 0;
}


void TeamAdapter::unloadTeams() {
    QList<Team> tmplist;
    teamlist = new TeamItemListModel(tmplist);
}


int TeamAdapter::getTeamCount() { return teamlist->rowCount(); }



QString TeamAdapter::getTeamFromID(QString id) {
    if(id.contains("/", Qt::CaseInsensitive)) {
        std::string stdstrid = id.toStdString();
        int lastslash = stdstrid.find_last_of("/");

        QString teamid = QString::fromStdString(stdstrid.substr(0, lastslash));

        return getTeamFromID(teamid);
    } else return teamlist->getTeamnameFromTeamID(id);
}


QString TeamAdapter::getNameFromID(QString id) {
    if(id.contains("/", Qt::CaseInsensitive)) {
        std::string stdstrid = id.toStdString();
        int lastslash = stdstrid.find_last_of("/");

        QString teamid = QString::fromStdString(stdstrid.substr(0, lastslash));
        QString mightBeAnInt = QString::fromStdString(stdstrid.substr(lastslash+1,stdstrid.length()));
        bool isAnInteger;
        int tmp = mightBeAnInt.toInt(&isAnInteger);

        if(isAnInteger && tmp > 0) return teamlist->getTeamMemberFromID(teamid, tmp);
        else return QString("?[").append(getTeamFromID(id)).append("]");
    } else return teamlist->getTeamnameFromTeamID(id);
}




QAbstractTableModel* TeamAdapter::getPerformersList(QString id) {
    QList<QString> choices;

    if(id == nullptr) {}
    else if(id.contains("/", Qt::CaseInsensitive)) choices.append(getNameFromID(id));
    else {
        choices.append("[" + getNameFromID(id) + "]");
        choices.append(teamlist->getTeamMembersFromID(id));
    }

    return new PeopleItemListModel(choices);
}

