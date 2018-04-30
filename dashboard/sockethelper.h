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


#ifndef SOCKETHELPER_H
#define SOCKETHELPER_H

#include <QObject>
#include <QUdpSocket>
#include <QMap>
#include "signalhelper.h"

class SocketHelper : public QObject
{
    Q_OBJECT
public:
    explicit SocketHelper(unsigned int port = 54545, QObject *parent = 0);
    ~SocketHelper();
    void setSignalHelper(unsigned int signature, SignalHelper* s);

signals:

private slots:
    void processDatagrams();

private:
    QUdpSocket *udpSocket;
    unsigned int port;
    QMap<unsigned int, SignalHelper*> sigmap;
};

#endif // SOCKETHELPER_H
