/****************************************************************************
**
** Copyright (C) 2018 Sebastian Friedl
** Contact: sfr682k@t-online.de
**
** $QT_BEGIN_LICENSE:GPL$
** GNU General Public License Usage
** This file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $QT_END_LICENSE$
**
****************************************************************************/


#include "broadcastlistmodel.h"


Broadcast::Broadcast(QString ipAdd, int prt, int i) {
    ipAddress = QHostAddress(ipAdd);
    port = prt;
    id = i;
}


QHostAddress Broadcast::getAddress() { return ipAddress; }
int          Broadcast::getPort()    { return port; }
int          Broadcast::getId()      { return id; }




BroadcastListModel::BroadcastListModel(QObject *parent) : QAbstractTableModel(parent) {
    QList<Broadcast> foo;
    listOfBcasts = foo;
}


int BroadcastListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return listOfBcasts.length();
}

int BroadcastListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;
}

QVariant BroadcastListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= listOfBcasts.length() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        Broadcast bcast = listOfBcasts.at(index.row());

        switch(index.column()) {
            case 0:  return bcast.getAddress().toString();
            case 1:  return bcast.getPort();
            case 2:  return bcast.getId();
            default: return QVariant();
        }
    }

    if (role == Qt::BackgroundRole) {
        if(index.row() == highlightedRow)
            return highlightColor;
    }

    return QVariant();
}


QVariant BroadcastListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:  return "IP Address";
            case 1:  return "Port";
            case 2:  return "ID";
            default: return QVariant();
        }
    }
    return QVariant();
}

/*
bool BroadcastListModel::insertRows(int position, int rows, const QModelIndex &index) {
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        Stage stage(QTime(0,0,1), QTime(0,0,0), " ");
        listOfStages.insert(position, stage);
    }

    endInsertRows();
    return true;
}
*/




bool BroadcastListModel::addBroadcast(QString ip, int prt, int i) {
    beginInsertRows(QModelIndex(), listOfBcasts.length(), listOfBcasts.length());
    listOfBcasts.append(Broadcast(ip, prt, i));
    endInsertRows();
    return true;
}


QList<Broadcast> BroadcastListModel::getListOfBcasts() { return listOfBcasts; }


