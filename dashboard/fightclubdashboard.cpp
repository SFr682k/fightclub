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


#include "fightclubdashboard.h"
#include "ui_fightclubdashboard.h"

#include "aboutdialog.h"
#include "clientboxwidget.h"

#include <QHBoxLayout>
#include <QListIterator>
#include <QSpacerItem>
#include <QTime>
#include <QVBoxLayout>


#include <QDebug>


FightclubDashboard::FightclubDashboard(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FightclubDashboard)
{
    ui->setupUi(this);

    container = ui->container;

    numberOfClocks = 0;


    listofclockboxes = QList<QGroupBox*>();
    listofperflabels = QList<QLabel*>();
    listofphaselabels = QList<QLabel*>();


    refreshtimer = new QTimer();
    refreshtimer->start(30);

    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateTimeDisplay()));

    switchpagetimer = new QTimer();
    switchpagetimer->setInterval(6000);


    mbcastclient = new MultiBroadcastClient(this);
    connect(mbcastclient, SIGNAL(newClock(SignalHelper*)), this, SLOT(createClock(SignalHelper*)));

    QTimer::singleShot(6000 - QTime::currentTime().msecsSinceStartOfDay() % 6000,
                       switchpagetimer, SLOT(start()));

    connect(switchpagetimer, SIGNAL(timeout()), this, SLOT(nextContainerPage()));


    aboutDialogOpen = false;

    mbcastclient->loadFromFile("fights.txt");
}


FightclubDashboard::~FightclubDashboard() {
    delete ui;
}


void FightclubDashboard::openAboutDialog() {
    if(!aboutDialogOpen) {
        aboutDialogOpen = true;
        AboutDialog *ad = new AboutDialog(this);
        ad->exec();
        aboutDialogOpen = false;
    }
}



void FightclubDashboard::createClock(SignalHelper* signalHelper) {
    if(numberOfClocks/5 >= container->count()) {
        // All pages of the clock container are full
        QWidget *newPage = new QWidget();

        QGridLayout *containerGrid = new QGridLayout();
        containerGrid->setMargin(0);
        containerGrid->setSpacing(8);
        containerGrid->setRowStretch(0,2);
        containerGrid->setRowStretch(1,2);
        containerGrid->setRowStretch(2,2);
        containerGrid->setRowStretch(3,2);
        containerGrid->setRowStretch(4,2);

        currentGrid = containerGrid;

        newPage->setLayout(containerGrid);
        container->addWidget(newPage);
    }

    ClientBoxWidget *clockbox = new ClientBoxWidget(signalHelper, this);
    currentGrid->addWidget(clockbox, numberOfClocks % 5, 0);

    connect(this, SIGNAL(screenSizeChanged(int)), clockbox, SLOT(onResizeEvent(int)));

    numberOfClocks++;
    ui->currentpagelabel->setText(QString::number(container->currentIndex() +1).append("/").append(QString::number(container->count())));
}





void FightclubDashboard::nextContainerPage() {
    if(container->currentIndex() < container->count() -1)
        container->setCurrentIndex(container->currentIndex() +1);
    else container->setCurrentIndex(0);

    if(container->count() > 0)
        ui->currentpagelabel->setText(QString::number(container->currentIndex() +1).append("/").append(QString::number(container->count())));
    else ui->currentpagelabel->setText(" ");
}



void FightclubDashboard::updateTimeDisplay() {
    QString currentTime = QTime::currentTime().toString("HH:mm");
    if(QTime::currentTime().second() % 2 != 0) currentTime[2] = ' ';
    ui->currenttime->display(currentTime);
}



void FightclubDashboard::resizeEvent(QResizeEvent *event) {
    QFont tnamefont = ui->tournamentname->font();
    tnamefont.setPointSize(height()*0.026);
    ui->tournamentname->setFont(tnamefont);

    QFont pagelabelfont = ui->currentpagelabel->font();
    pagelabelfont.setPointSize(height()*0.02);
    ui->currentpagelabel->setFont(pagelabelfont);

    emit screenSizeChanged(height());

    QWidget::resizeEvent(event);
}



void FightclubDashboard::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_F1:
            openAboutDialog();
            break;

        case Qt::Key_F:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                setWindowState(Qt::WindowFullScreen);
            break;

        case Qt::Key_Escape:
            if(windowState() == Qt::WindowFullScreen) setWindowState(Qt::WindowMaximized);
            break;

        case Qt::Key_Q:
            if(QApplication::keyboardModifiers() & Qt::ControlModifier)
                this->close();
            break;

        default:
            QWidget::keyPressEvent(event);
    }
}
