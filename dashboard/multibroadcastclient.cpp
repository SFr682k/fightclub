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


#include "multibroadcastclient.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QString>
#include <QPair>

MultiBroadcastClient::MultiBroadcastClient(QObject *parent) :
    QObject(parent)
{
    mp.clear();
}


MultiBroadcastClient::~MultiBroadcastClient() {
    for(QMap<unsigned int,SocketHelper*>::Iterator i = mp.begin(); i != mp.end() ; i++)
        delete i.value();
}


void MultiBroadcastClient::loadFromFile(QString path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    mp.clear();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList sl = line.split(QChar('\t'));
        if(sl.size() == 3){
             int port = sl.value(0).toInt();
             int signature = sl.value(1).toInt();
             QString title = sl.at(2);
             SignalHelper *sh = new SignalHelper(title);

             if (mp.contains(port))
                 mp[port]->setSignalHelper(signature, sh);
             else {
                 mp.insert(port, new SocketHelper(port));
                 mp[port]->setSignalHelper(signature, sh);
             }

             emit newClock(sh);
        }
    }

    file.close();
}


void MultiBroadcastClient::unloadList() { mp.clear(); }
