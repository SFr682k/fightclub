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


#ifndef TEAMITEMLISTMODEL_H
#define TEAMITEMLISTMODEL_H

#include <QAbstractTableModel>


class Team {
public:
    Team(QString, QString, QList<QString>);
    QString getTeamID() const;
    QString getTeamname() const;

private:
    QString teamid;
    QString teamname;
    QList<QString> teammembers;
};



class TeamItemListModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit TeamItemListModel(QObject *parent = nullptr);
    explicit TeamItemListModel(QList<Team>, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QString getTeamnameFromTeamID(QString);

private:
    QList<Team> listofteams;
};

#endif // TEAMITEMLISTMODEL_H
