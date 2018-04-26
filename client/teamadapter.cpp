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
