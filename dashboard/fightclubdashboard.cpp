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
#include "themeclockwidget.h"

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


    refreshtimer = new QTimer();
    refreshtimer->start(30);

    connect(refreshtimer, SIGNAL(timeout()), this, SLOT(updateTimeDisplay()));


    switchpagetimer = new QTimer();
    switchpagetimer->setInterval(6000);

    QTimer::singleShot(6000 - QTime::currentTime().msecsSinceStartOfDay() % 6000,
                       switchpagetimer, SLOT(start()));

    connect(switchpagetimer, SIGNAL(timeout()), this, SLOT(nextContainerPage()));


    aboutDialogOpen = false;

    createClock();
    createClock();
    createClock();
    createClock();
    createClock();
    createClock();
    createClock();
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



void FightclubDashboard::createClock() {
    if(numberOfClocks/6 >= container->count()) {
        // All pages of the clock container are full
        QWidget *newPage = new QWidget();

        QGridLayout *containerGrid = new QGridLayout();
        containerGrid->setMargin(0);
        containerGrid->setSpacing(0);
        containerGrid->setColumnStretch(0,1);
        containerGrid->setColumnStretch(1,1);
        containerGrid->setRowStretch(0,1);
        containerGrid->setRowStretch(1,1);
        containerGrid->setRowStretch(2,1);

        currentGrid = containerGrid;

        newPage->setLayout(containerGrid);
        container->addWidget(newPage);
    }


    QGroupBox *clockbox = new QGroupBox();
    QFont clockboxfont = clockbox->font();
    clockboxfont.setPointSize(height()*0.019);
    clockbox->setFont(clockboxfont);
    listofclockboxes.append(clockbox);
    clockbox->setTitle(QString::number(numberOfClocks));


    QGridLayout *clkboxlayout = new QGridLayout();
    clkboxlayout->setColumnStretch(0,3);
    clkboxlayout->setColumnStretch(1,1);
    clockbox->setLayout(clkboxlayout);


    QVBoxLayout *stagestatusbox = new QVBoxLayout();
    stagestatusbox->setSpacing(0);
    clkboxlayout->addLayout(stagestatusbox,0,0);


    QLabel *performerslabel = new QLabel();
    QFont performerslabelfont = performerslabel->font();
    performerslabelfont.setPointSize(height()*0.019);
    performerslabel->setFont(performerslabelfont);
    listofperflabels.append(performerslabel);
    performerslabel->setWordWrap(true);
    performerslabel->setText("foobar");
    performerslabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    performerslabel->setAlignment(Qt::AlignBottom);
    stagestatusbox->addWidget(performerslabel);


    QLabel *phaselabel = new QLabel();
    QFont phaselabelfont = phaselabel->font();
    phaselabelfont.setPointSize(height()*0.02);
    phaselabelfont.setBold(true);
    phaselabel->setFont(phaselabelfont);
    listofphaselabels.append(phaselabel);
    phaselabel->setWordWrap(true);
    phaselabel->setText("foobaz");
    phaselabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    phaselabel->setAlignment(Qt::AlignTop);
    stagestatusbox->addWidget(phaselabel);

    QHBoxLayout *eltimelayout = new QHBoxLayout();
    stagestatusbox->addLayout(eltimelayout);

    QSpacerItem *eltimespacer = new QSpacerItem(6,6,QSizePolicy::Expanding,QSizePolicy::Fixed);
    eltimelayout->addItem(eltimespacer);

    QLCDNumber *eltimedisplay = new QLCDNumber();
    eltimedisplay->setMinimumSize(90,27);
    eltimedisplay->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
    eltimedisplay->setDigitCount(7);
    eltimedisplay->setFrameShape(QFrame::NoFrame);
    eltimedisplay->setSegmentStyle(QLCDNumber::SegmentStyle::Flat);
    eltimedisplay->display(" ");
    eltimelayout->addWidget(eltimedisplay);


    ThemeClockWidget *clockwidget = new ThemeClockWidget();
    clockwidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    clockwidget->setMinimumWidth(110);
    clockwidget->setRenderHints({QPainter::Antialiasing, QPainter::SmoothPixmapTransform});
    clockwidget->setRoomclock(true);
    clkboxlayout->addWidget(clockwidget,0,1);


    connect(refreshtimer, SIGNAL(timeout()), clockwidget, SLOT(act()));


    currentGrid->addWidget(clockbox, (numberOfClocks % 6)/2, (numberOfClocks % 6) % 2);

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


    QListIterator<QGroupBox*> cboxiterator(listofclockboxes);
    while(cboxiterator.hasNext()) {
        QGroupBox *clockbox = cboxiterator.next();
        QFont clockboxfont = clockbox->font();
        clockboxfont.setPointSize(height()*0.019);
        clockbox->setFont(clockboxfont);
    }

    QListIterator<QLabel*> perfliterator(listofperflabels);
    while(perfliterator.hasNext()) {
        QLabel *perflabel = perfliterator.next();
        QFont perflabelfont = perflabel->font();
        perflabelfont.setPointSize(height()*0.019);
        perflabel->setFont(perflabelfont);
    }

    QListIterator<QLabel*> phaseliterator(listofphaselabels);
    while(phaseliterator.hasNext()) {
        QLabel *phaselabel = phaseliterator.next();
        QFont phaselabelfont = phaselabel->font();
        phaselabelfont.setPointSize(height()*0.02);
        phaselabel->setFont(phaselabelfont);
    }

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
