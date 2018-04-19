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


#ifndef STAGELISTMODEL_H
#define STAGELISTMODEL_H


#include <QAbstractTableModel>
#include <QColor>
#include <QTime>


class Stage {
public:
    Stage(QString rcstg = nullptr, QString label = nullptr, int p = -1, QString rep = nullptr, QString opp = nullptr, QString rev = nullptr);
    QString getRoomclockstage();
    QString getLabel();
    int getProblem();
    QString getReporterID();
    QString getOpponentID();
    QString getReviewerID();

private:
    QString roomclockstage;
    QString label;
    int problem;
    QString reporterID;
    QString opponentID;
    QString reviewerID;

};



class StageListModel : public QAbstractTableModel {
    Q_OBJECT

public:
    StageListModel(QObject *parent=0);
    StageListModel(QList<Stage>, QObject *parent=0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setHighlightedRow(int);
    QList<Stage> getStageList();


private:
    QList<Stage> listofstages;
    int highlightedRow;
    QColor highlightColor;
};

#endif // STAGELISTMODEL_H
