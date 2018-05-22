/****************************************************************************
**
** Copyright (C) 2018 Sebastian Friedl
** Contact: sfr682k@t-online.de
**
** Based on the QIYPTClock Broadcast Server
** Copyright (C) 2011-2012 Jan M. Binder, Simeon Voelkel
** Contact: jan.binder@sfz-bw.de, simeon.voelkel@sfz-bw.de
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

#ifndef BROADCASTSERVER_H
#define BROADCASTSERVER_H

#include <QObject>
#include <QtNetwork/QUdpSocket>

#include "broadcastlistmodel.h"


class BroadcastServer : public QObject
{
    Q_OBJECT
public:
    explicit BroadcastServer(QObject *parent = 0);
    ~BroadcastServer();
    void emitModel();

    Broadcast getBroadcast(int);

private:
    BroadcastListModel *broadcastlistmodel;
    QUdpSocket *udpSocket;
    QHostAddress broadcastAddress;
    QString phasename, problem, performers;
    int elapsedTime, maximumTime;
    bool roomclock;
    unsigned int port, signature;

signals:
    void socketStatus();
    void bcastRequest();
    void bcastTableModelChanged(QAbstractTableModel*);

public slots:
    void updatePhaseName(QString);
    void updateProblem(QString);
    void updatePerformers(QString);
    void updateElapsedTime(int);
    void updateMaximumTime(int);
    void updateRClockState(bool roomclock);
    void setBroadcastAddress(QString);
    void setBroadcastPort(unsigned int);
    void setSignature(unsigned int);

    void addBcast(QString, int, int);

private slots:
    void broadcast();

};

#endif // BROADCASTSERVER_H
