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


#include "problemitemlistmodel.h"


#include <QDebug>

Problem::Problem(int problemnr, QString problemtitle) {
    number = problemnr;
    title = problemtitle;
}


int Problem::getNumber() const { return number; }
QString Problem::getTitle() { return title; }





ProblemItemListModel::ProblemItemListModel(QObject *parent) : QAbstractTableModel(parent) { }

ProblemItemListModel::ProblemItemListModel(QList<Problem> problems, QObject *parent) : QAbstractTableModel(parent)
    { listofproblems = problems; }



int ProblemItemListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return listofproblems.length();
}


int ProblemItemListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 3;
}


QVariant ProblemItemListModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid()) return QVariant();
    if(index.row() >= listofproblems.size() || index.row() < 0) return QVariant();

    if(role == Qt::DisplayRole) {
        Problem problem = listofproblems.at(index.row());

        switch(index.column()) {
            default:
                return QString::number(problem.getNumber()) + ". " + problem.getTitle();
                break;
            case 1:
                return problem.getNumber();
                break;
            case 2:
                return problem.getTitle();
                break;
        }
    }

    return QVariant();
}


Problem ProblemItemListModel::getProblem(int nr) {
    for(int i = 0; i < listofproblems.length(); i++) {
        if(listofproblems.at(i).getNumber() == nr) return listofproblems.at(i);
    }

    return Problem(nr, QString("???"));
}
