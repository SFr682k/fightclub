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


#ifndef BROADCASTLISTMODEL_H
#define BROADCASTLISTMODEL_H

#include <QObject>

#include <QAbstractTableModel>
#include <QColor>
#include <QHostAddress>


class Broadcast {
public:
    Broadcast(QString ipAdd = "127.0.0.1", int prt = 45454, int i = 12345);
    QHostAddress getAddress();
    int getPort();
    int getId();

private:
    QHostAddress ipAddress;
    int port;
    int id;
};


class BroadcastListModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit BroadcastListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool addBroadcast(QString ip = "127.0.0.1", int prt = 45454, int i = 12345);
    bool editBroadcast(int, QString, int, int);
    void deleteBroadcast(int);

    QList<Broadcast> getListOfBcasts();

private:
    QList<Broadcast> listOfBcasts;
    int highlightedRow;
    QColor highlightColor;
};

#endif // BROADCASTLISTMODEL_H
