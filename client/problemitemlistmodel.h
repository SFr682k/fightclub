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


#ifndef PROBLEMITEMLISTMODEL_H
#define PROBLEMITEMLISTMODEL_H

#include <QAbstractTableModel>

class Problem {
public:
    Problem(int, QString);
    int getNumber();
    QString getTitle();

private:
    int number;
    QString title;
};



class ProblemItemListModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit ProblemItemListModel(QObject *parent = nullptr);
    explicit ProblemItemListModel(QList<Problem>, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QList<Problem> listofproblems;


};

#endif // PROBLEMITEMLISTMODEL_H
