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


#include "problemadapter.h"

#include <QFile>
#include <QList>
#include <QTextStream>


ProblemAdapter::ProblemAdapter(QObject *parent) : QObject(parent) {
    problemlist = new ProblemItemListModel();
}



int ProblemAdapter::loadProblemsFromFile(QString path) {
    QList<Problem> tmplist;
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 1;

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList splitline = line.split('\t');


        if(splitline.size() > 1)
            tmplist.append(Problem(splitline.value(0).toInt(), splitline.value(1)));
    }

    problemlist = new ProblemItemListModel(tmplist);
    file.close();


    return 0;
}


void ProblemAdapter::unloadProblemsList() {
    QList<Problem> tmplist;
    problemlist = new ProblemItemListModel(tmplist);
}


QAbstractTableModel* ProblemAdapter::getProblemList(int nr) {
    QList<Problem> tmplist;

    if(nr >= 0) {
        tmplist.append(problemlist->getProblem(nr));
        return new ProblemItemListModel(tmplist);
    } else if(nr == -1) return problemlist;

    return new ProblemItemListModel(tmplist);
}


int ProblemAdapter::getProblemCount() { return problemlist->rowCount(); }
