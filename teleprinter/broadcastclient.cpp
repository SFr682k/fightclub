/****************************************************************************
**
** Copyright (C) 2018 Sebastian Friedl
** Contact: sfr682k@t-online.de
**
** Based on the QIYPTClockClient Broadcast Client
** available on https://github.com/drogenlied/qiyptclock
** Copyright (C) 2011-2012 Jan M. Binder, Simeon Voelkel
** Contact: jan.binder@sfz-bw.de, simeon.voelkel@sfz-bw.de
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

#include "broadcastclient.h"
#include <QDataStream>
#include <QtNetwork>


BroadcastClient::BroadcastClient(QObject *parent, unsigned int prt, unsigned int i) :
    QObject(parent)
{
    if(prt > 0) port = prt%65536;
    else        port = 45454;

    id = i;

    elapsedTime = 0;
    maximumTime = 1;
    roomclock = 2;
    phasename = "";
    problem = "";
    performers = "";

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port, QUdpSocket::ShareAddress);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processDatagrams()));
}


BroadcastClient::~BroadcastClient() {
    delete udpSocket;
}


void BroadcastClient::setListeningPort(unsigned int prt) {
    udpSocket->close();
    if(prt > 0) port = prt%65536;
    else        port = 45454;

    udpSocket->bind(port, QUdpSocket::ShareAddress);
}

void BroadcastClient::setID(unsigned int i) { id = i; }



void BroadcastClient::processDatagrams() {
    quint32 nid, nmaximumTime, nroomclock, nelapsedTime;
    QString nphasename, nproblem, nperformers;

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QDataStream dgstream(&datagram, QIODevice::ReadOnly);
        dgstream >> nid;
        dgstream >> nmaximumTime;
        dgstream >> nelapsedTime;
        dgstream >> nroomclock;
        dgstream >> nphasename;
        dgstream >> nproblem;
        dgstream >> nperformers;
    }

    if(id != nid) return;

    if(roomclock != nroomclock) {
        roomclock = nroomclock;
        emit roomClockChanged(roomclock > 0);
        emit elapsedTimeUpdate(0);
    }

    if(elapsedTime != nelapsedTime) {
        elapsedTime = nelapsedTime;
        emit elapsedTimeUpdate(elapsedTime);
    }

    if(maximumTime != nmaximumTime) {
        maximumTime = nmaximumTime;
        emit maximumTimeChanged(maximumTime);
    }

    if(phasename != nphasename) {
        phasename = nphasename;
        emit phaseNameChanged(phasename);
    }

    if(nproblem == "") { nproblem = " "; }
    if(problem != nproblem) {
        problem = nproblem;
        emit problemChanged(problem);
    }

    if(performers != nperformers) {
        performers = nperformers;
        emit performersChanged(performers);
    }
}



uint BroadcastClient::getBcastPort() { return port; }
uint BroadcastClient::getBcastID()   { return id; }
