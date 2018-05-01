/****************************************************************************
**
** Copyright (C) 2018 Sebastian Friedl
** Contact: sfr682k@t-online.de
**
** Based on the QIYPTClock MultiBroadcastClient
** available on https://github.com/drogenlied/qiyptclock
** Copyright (C) 2011-2012 Jan M. Binder
** Contact: jan.binder@sfz-bw.de
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


#include "sockethelper.h"
#include <QDataStream>
#include <QtNetwork>

SocketHelper::SocketHelper(unsigned int p, QObject *parent) :
    QObject(parent)
{
    if(p > 0) port = p%65536;
    else      port = 54545;

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(port, QUdpSocket::ShareAddress);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processDatagrams()));
}


SocketHelper::~SocketHelper() {
    for(QMap<unsigned int,SignalHelper*>::Iterator i = sigmap.begin(); i != sigmap.end() ; i++)
        delete i.value();

    sigmap.clear();
    delete udpSocket;
}


void SocketHelper::setSignalHelper(unsigned int signature, SignalHelper* s) {
    if(sigmap.contains(signature)) {
        // TODO: Emit warning (same port, same signature)
        sigmap[signature] = s;
    } else sigmap.insert(signature, s);
}


void SocketHelper::processDatagrams() {
    quint32 nsignature, nallowedtime, ntime, nroomclock;;
    QString nphasename, nproblem, nperformers;

    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());
        QDataStream dgstream(&datagram, QIODevice::ReadOnly);
        dgstream >> nsignature;
        dgstream >> nallowedtime;
        dgstream >> ntime;
        dgstream >> nroomclock;
        dgstream >> nphasename;
        dgstream >> nproblem;
        dgstream >> nperformers;

        if (sigmap.contains(nsignature))
            sigmap[nsignature]->emitSignal(ntime, nallowedtime, nroomclock,
                                           nphasename, nproblem, nperformers);
    }
}
