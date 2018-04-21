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

#ifndef BROADCASTCLIENT_H
#define BROADCASTCLIENT_H

#include <QObject>
#include <QtNetwork/QUdpSocket>

class BroadcastClient : public QObject
{
    Q_OBJECT
public:
    explicit BroadcastClient(QObject *parent = 0, unsigned int port = 45454, unsigned int signature = 12345);
    ~BroadcastClient();
    QString toString();

signals:
    void phaseNameChanged(QString);
    void problemChanged(QString);
    void performersChanged(QString);
    void elapsedTimeUpdate(int);
    void maximumTimeChanged(int);
    void roomClockChanged(bool);

public slots:
    void setListeningPort(unsigned int);
    void setSignature(unsigned int);

private slots:
    void processDatagrams();

private:
    QUdpSocket *udpSocket;
    unsigned int port;
    QString phasename;
    QString problem;
    QString performers;
    quint32 elapsedTime;
    quint32 maximumTime;
    quint32 roomclock;
    quint32 signature;
};

#endif // BROADCASTCLIENT_H
