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


#include "stagelistmodel.h"


Stage::Stage(QString rcstring, QString lbl, int p, QString rep, QString opp, QString rev) {
    roomclockstage = rcstring;
    label = lbl;
    problem = p;
    reporterID = rep;
    opponentID = opp;
    reviewerID = rev;
}


QString Stage::getRoomclockstage() const { return roomclockstage; }
QString Stage::getLabel()                { return label; }
int     Stage::getProblem()              { return problem; }
QString Stage::getReporterID()           { return reporterID; }
QString Stage::getOpponentID()           { return opponentID; }
QString Stage::getReviewerID()           { return reviewerID; }



StageListModel::StageListModel(QObject *parent) : QAbstractTableModel(parent)  {
    highlightColor = QColor::fromRgb(0xff, 0x99, 0x11);
    highlightedRow = -1;
}

StageListModel::StageListModel(QList<Stage> stages, QObject *parent) : QAbstractTableModel(parent) {
    highlightColor = QColor::fromRgb(0xff, 0x99, 0x11);
    listofstages = stages;
    highlightedRow = -1;
}

void StageListModel::setTeamAdapter(TeamAdapter* teamadapt) { teamadapter = teamadapt; }



int StageListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return listofstages.size();
}

int StageListModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 6;
}

QVariant StageListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= listofstages.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        Stage stage = listofstages.at(index.row());

        if(index.column() == 0) {
            if(stage.getRoomclockstage() == nullptr) {
                QString stageoview;
                stageoview = stage.getLabel();
                if(teamadapter != nullptr) {
                    if(stage.getReporterID() != nullptr) {
                        stageoview.append(QString("  —  "));
                        stageoview.append(teamadapter->getTeamFromID(stage.getReporterID()));
                    }
                    if(stage.getOpponentID() != nullptr) {
                        stageoview.append(QString("  <>  "));
                        stageoview.append(teamadapter->getTeamFromID(stage.getOpponentID()));
                    }
                    if(stage.getReviewerID() != nullptr) {
                        stageoview.append(QString("  <>  "));
                        stageoview.append(teamadapter->getTeamFromID(stage.getReviewerID()));
                    }
                }
                return stageoview;
            } else return stage.getRoomclockstage();}
        else if(index.column() == 1) return stage.getRoomclockstage();
        else if(index.column() == 2) return stage.getProblem();
        else if(index.column() == 3) return stage.getReporterID();
        else if(index.column() == 4) return stage.getOpponentID();
        else if(index.column() == 5) return stage.getReviewerID();
    }

    if (role == Qt::BackgroundRole) {
        if(index.row() == highlightedRow)
            return highlightColor;
    }

    return QVariant();
}






void StageListModel::setHighlightedRow(int row){
    if (row < listofstages.size()){
        int tlRow = (row < highlightedRow) ? row : highlightedRow;
        int brRow = (row < highlightedRow) ? highlightedRow : row;
        highlightedRow = row;
        emit dataChanged(this->index(tlRow,0),this->index(brRow,0));
    }
}



QList<Stage> StageListModel::getStageList() {
    return listofstages;
}
