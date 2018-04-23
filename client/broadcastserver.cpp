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

#include "broadcastserver.h"
#include <QDataStream>
#include <QtNetwork/QUdpSocket>

#include <QDebug>

BroadcastServer::BroadcastServer(QObject *parent, QHostAddress addr, unsigned int prt, unsigned int sig) :
    QObject(parent)
{
    udpSocket = nullptr;
    broadcastEnabled = false;

    if(prt > 0) port = prt%65536;
    else        port = 45454;

    elapsedTime = 0, maximumTime = 0;
    phasename = " ", problem = " ", performers = " ";

    signature = sig;
    broadcastAddress = addr;

    connect(this, SIGNAL(bcastRequest()), this, SLOT(broadcast()));
}


BroadcastServer::~BroadcastServer(){
    delete udpSocket;
}


void BroadcastServer::enableBroadcast(bool enable) {
    broadcastEnabled = enable;
    if(broadcastEnabled) bcastRequest();
}


void BroadcastServer::updatePhaseName(QString name) {
    phasename = name;
    emit bcastRequest();
}


void BroadcastServer::updateProblem(QString prob) {
    problem = prob;
    emit bcastRequest();
}


void BroadcastServer::updateElapsedTime(int t) {
    elapsedTime = t;
    emit bcastRequest();
}


void BroadcastServer::updateMaximumTime(int t) {
    if(t > 0) maximumTime = t;
    emit bcastRequest();
}


void BroadcastServer::updateRClockState(bool roomclock) {
    this->roomclock = roomclock;
    emit bcastRequest();
}


void BroadcastServer::setBroadcastAddress(QString ipstr) {
    broadcastAddress = QHostAddress(ipstr);
    emit bcastRequest();
}


void BroadcastServer::setBroadcastPort(unsigned int prt) {
    if (prt > 0) port = prt%65536;
    emit bcastRequest();
}


void BroadcastServer::setSignature(unsigned int sig) {
    signature = sig;
    emit bcastRequest();
}


void BroadcastServer::broadcast() {
    if(!broadcastEnabled) return;

    if(udpSocket == nullptr) udpSocket = new QUdpSocket(this);

    QByteArray datagram;
    QDataStream dgstream(&datagram, QIODevice::ReadWrite);
    dgstream << (quint32)signature;
    dgstream << (quint32)maximumTime;
    dgstream << (quint32)elapsedTime;
    dgstream << (quint32)(roomclock? 1 : 0);
    dgstream << phasename;
    dgstream << problem;
    dgstream << performers;
    udpSocket->writeDatagram(datagram.data(), datagram.size(), broadcastAddress, port);
}
