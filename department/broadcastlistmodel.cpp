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
    QList<Broadcast> loB;
    listOfBcasts = loB;
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





bool BroadcastListModel::addBroadcast(QString ip, int prt, int i) {
    QListIterator<Broadcast> iterator(listOfBcasts);

    while(iterator.hasNext()) {
        Broadcast bcast = iterator.next();

        if((bcast.getAddress().toString() == ip) && (bcast.getPort() == prt) && (bcast.getId() == i))
            return false;
    }

    beginInsertRows(QModelIndex(), listOfBcasts.length(), listOfBcasts.length());
    listOfBcasts.append(Broadcast(ip, prt, i));
    endInsertRows();
    return true;
}


bool BroadcastListModel::editBroadcast(int row, QString newip, int newprt, int newid) {
    if(!(row < listOfBcasts.size())) {
        return false;
    }


    Broadcast bcast = listOfBcasts.at(row);
    if((bcast.getAddress().toString() == newip)
            && (bcast.getPort() == newprt) && (bcast.getId() == newid))
        return true; // no changes
    else {
        QListIterator<Broadcast> iterator(listOfBcasts);

        while(iterator.hasNext()) {
            Broadcast bcast = iterator.next();

            if((bcast.getAddress().toString() == newip) && (bcast.getPort() == newprt) && (bcast.getId() == newid))
                return false; // the ip/port/id combination is already occupied
        }
    }


    listOfBcasts.replace(row, Broadcast(newip, newprt, newid));
    emit dataChanged(this->index(row,0), this->index(row,2));
    return true;
}


void BroadcastListModel::deleteBroadcast(int row) {
    if(!(row < listOfBcasts.size())) return;

    beginRemoveRows(QModelIndex(), row, row);
    listOfBcasts.takeAt(row);
    endRemoveRows();
}





QList<Broadcast> BroadcastListModel::getListOfBcasts() { return listOfBcasts; }

