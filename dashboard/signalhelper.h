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


#ifndef SIGNALHELPER_H
#define SIGNALHELPER_H

#include <QObject>
#include <QString>

class SignalHelper : public QObject
{
    Q_OBJECT
public:
    explicit SignalHelper( QString title = QString("No Title set"), QObject *parent = 0);
    ~SignalHelper();
    void emitSignal(quint32, quint32, quint32, QString, QString, QString);
    QString getTitle();
    QString toString();

signals:
    void maximumTimeChanged(int);
    void elapsedTimeUpdate(int);
    void phaseNameChanged(QString);
    void problemChanged(QString);
    void performersChanged(QString);
    void roomclockChanged(bool);

public slots:

private:
    QString title;
    QString phaseName, problem, performers;
    int elapsedTime, maximumTime;
    uint roomclock;

};

#endif // SIGNALHELPER_H
