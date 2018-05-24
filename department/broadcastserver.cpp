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
** Foundation and appearing in the file LICENSE.md included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "broadcastserver.h"

#include <QDataStream>
#include <QMessageBox>
#include <QUdpSocket>



BroadcastServer::BroadcastServer(QWidget *parentWidget, QObject *parent) :
    QObject(parent)
{
    this->parent = parentWidget;

    udpSocket = nullptr;

    broadcastlistmodel = new BroadcastListModel();
    bcastproxymodel = new QSortFilterProxyModel();
    bcastproxymodel->setSourceModel(broadcastlistmodel);

    elapsedTime = 0, maximumTime = 0;
    phasename = " ", problem = " ", performers = " ";


    connect(this, SIGNAL(bcastRequest()), this, SLOT(broadcast()));
}


BroadcastServer::~BroadcastServer(){
    delete udpSocket;
    delete broadcastlistmodel;
    delete bcastproxymodel;
}


void BroadcastServer::emitModel() { emit bcastTableModelChanged(bcastproxymodel); }




void BroadcastServer::updatePhaseName(QString name) {
    phasename = name;
    emit bcastRequest();
}


void BroadcastServer::updateProblem(QString prob) {
    if(prob == nullptr) prob = " ";
    problem = prob;
    emit bcastRequest();
}


void BroadcastServer::updatePerformers(QString perf) {
    if(perf == nullptr) perf = " ";
    performers = perf;
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





void BroadcastServer::broadcast() {
    if(bcastproxymodel->rowCount() < 1) return;

    if(udpSocket == nullptr) udpSocket = new QUdpSocket(this);

    QList<Broadcast> listOfBcasts = broadcastlistmodel->getListOfBcasts();
    QListIterator<Broadcast> iterator(listOfBcasts);

    while(iterator.hasNext()) {
        Broadcast bcast = iterator.next();

        QByteArray datagram;
        QDataStream dgstream(&datagram, QIODevice::ReadWrite);
        dgstream << (quint32)bcast.getId();
        dgstream << (quint32)maximumTime;
        dgstream << (quint32)elapsedTime;
        dgstream << (quint32)(roomclock? 1 : 0);
        dgstream << phasename;
        dgstream << problem;
        dgstream << performers;

        udpSocket->writeDatagram(datagram.data(), datagram.size(),
                                 bcast.getAddress(), bcast.getPort());
    }
}



void BroadcastServer::addBroadcast(QString ip, int port, int id) {
    if(broadcastlistmodel->addBroadcast(ip, port, id)) {
        emit bcastTableModelChanged(bcastproxymodel);
        emit bcastRequest();
    } else QMessageBox::critical(parent, "Broadcast already present",
                                 QString("Can’t add a broadcast to %1, port %2, ID %3:\n")
                                     .append("A broadcast to this IP, Port and ID is already present")
                                     .arg(ip).arg(QString::number(port)).arg(QString::number(id)));
}

void BroadcastServer::deleteBroadcast(QModelIndex idx) {
    broadcastlistmodel->deleteBroadcast(bcastproxymodel->mapToSource(idx).row());
    emit bcastTableModelChanged(bcastproxymodel);
}

void BroadcastServer::editBroadcast(QModelIndex srtidx, QString newip, int newport, int newid) {
    if(broadcastlistmodel->editBroadcast(bcastproxymodel->mapToSource(srtidx).row(), newip, newport, newid)) {
        emit bcastTableModelChanged(bcastproxymodel);
        emit bcastRequest();
    } else QMessageBox::critical(parent, "Broadcast conflict",
                                 QString("Can’t edit the current broadcast:\n")
                                     .append("A broadcast to %1, port %2, ID %3 is already present")
                                     .arg(newip).arg(QString::number(newport)).arg(QString::number(newid)));
}



Broadcast BroadcastServer::getBroadcast(QModelIndex srtidx) {
    QList<Broadcast> lOB = broadcastlistmodel->getListOfBcasts();
    if(srtidx.row() < lOB.length()) {
        return lOB.at(bcastproxymodel->mapToSource(srtidx).row());
    } else return Broadcast();
}
