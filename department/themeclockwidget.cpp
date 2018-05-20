/****************************************************************************
**
** Copyright (C) 2018 Sebastian Friedl
** Contact: sfr682k@t-online.de
**
** Based on the QIYPTClockClient ThemeClockWidget
** available on https://github.com/drogenlied/qiyptclock
** Copyright (C) 2011-2012 Jan M. Binder, Felix Engelmann, Simeon Voelkel
** Contact: jan.binder@sfz-bw.de, felix.engelmann@sfz-bw.de,
**          simeon.voelkel@sfz-bw.de
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

#include "themeclockwidget.h"
#include <cmath>
#include <QTime>


ThemeClockWidget::ThemeClockWidget(QWidget *parent) :
    QGraphicsView(parent)
{
    nscene = new QGraphicsScene();
    setScene(nscene);

    fg = new QGraphicsEllipseItem(0,0,300,300);
    fg->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    fg->setStartAngle(90*16);
    fg->setSpanAngle(-50*16);
    fg->setBrush(QBrush(QColor(100,30,100)));
    fg->setPen(QPen(QColor(0,0,0,0)));

    mg = new QGraphicsEllipseItem(0,0,300,300);
    mg->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    mg->setStartAngle(90*16);
    mg->setSpanAngle(-5*16);
    mg->setBrush(QBrush(QColor(200,0,0)));
    mg->setPen(QPen(QColor(0,0,0,0)));


    bg = new QGraphicsEllipseItem(0,0,300,300);
    bg->setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
    bg->setStartAngle(90*16);
    bg->setSpanAngle(-130*16);
    bg->setBrush(QBrush(QColor(50,200,30)));
    bg->setPen(QPen(QColor(0,0,0,0)));

    ring = new QGraphicsEllipseItem(0,0,300,300);
    ring->setPen(QPen(QColor(0,0,0,255)));
    ring->setBrush(QBrush(QColor(255,255,255)));

    focus = new QGraphicsEllipseItem(0,0,300,300);
    focus->setPen(QPen(QColor(0,0,0,0)));

    nscene->addItem(focus);
    nscene->addItem(ring);
    nscene->addItem(bg);
    nscene->addItem(mg);
    nscene->addItem(fg);

    time = 0;
    maxtime = 1;
    roomclock = false;

    rscene = new QGraphicsScene();

    QPen handPen;
    handPen.setColor(QColor(127, 160, 72, 100));
    handPen.setWidth(0);

    QPolygonF hourPolygon;
    hourPolygon << QPointF(-7, -25) << QPointF(-7, 84) << QPointF(0, 91) << QPointF(7, 84) << QPointF(7, -25);
    hourHand = new QGraphicsPolygonItem(hourPolygon);
    hourHand->setPen(handPen);
    hourHand->setBrush(QBrush(QColor(0,0,50)));
    hourHand->setPos(150,150);

    QPolygonF minutePolygon;
    minutePolygon << QPointF(-6, -38) << QPointF(-6, 138) << QPointF(0, 144) << QPointF(6, 138) << QPointF(6, -38);
    minuteHand = new QGraphicsPolygonItem(minutePolygon);
    minuteHand->setPen(handPen);
    minuteHand->setBrush(QBrush(QColor(0,0,50)));
    minuteHand->setPos(150,150);

    QPolygonF tipPolygon;
    tipPolygon << QPointF(2.5, 97) << QPointF(-2.5, 97) << QPointF(-1.5, 144) << QPointF(1.5, 144) ;

    secondHand=new QGraphicsPolygonItem(tipPolygon);
    secondHand->setPen(QPen(QColor(0,0,0,0)));
    secondHand->setBrush(QBrush(QColor(180,0,0)));
    secondHand->setPos(150,150);

    QPolygonF basePolygon;
    basePolygon << QPointF(2.5, 63) << QPointF(-2.5, 63) << QPointF(-4, -40) << QPointF(4, -40);

    secondHandBase=new QGraphicsPolygonItem(basePolygon);
    secondHandBase->setBrush(QBrush(QColor(180,0,0)));
    secondHandBase->setPen(QPen(QColor(0,0,0,0)));
    secondHandBase->setPos(150,150);

    secondRing = new QGraphicsEllipseItem(-12.5,67.5,25,25);//36 durchmesser 10 strich
    /*
      spitze: innen: 5
      außen:3
      zeiger:außen: 5
      innen: 8
      mitte 80 weg
      */
    QPen ringPen = QPen(QColor(180,0,0));
    ringPen.setWidthF(10);
    ringPen.setCapStyle(Qt::RoundCap);
    secondRing->setPen(ringPen);
    secondRing->setPos(150,150);
    
    focus2 = new QGraphicsEllipseItem(0,0,300,300);
    focus2->setPen(QPen(QColor(0,0,0,0)));
    rscene->addItem(focus2);

    QGraphicsRectItem *tmp = 0;

    for (int i = 0; i < 60 ; ++i) {
        if (i % 15 == 0)     tmp = new QGraphicsRectItem(-6,150,12,-46);
        else if (i % 5 == 0) tmp = new QGraphicsRectItem(-6,150,12,-37);
        else                 tmp = new QGraphicsRectItem(-2,150,4,-12);
        
        tmp->setBrush(QBrush(QColor(0,0,50)));
        tmp->setPen(QPen(QColor(0,0,0,0)));
        tmp->setPos(150,150);
        tmp->setRotation(6.0*i+0.001);
        rscene->addItem(tmp);
    }

    rscene->addItem(hourHand);
    rscene->addItem(minuteHand);
    rscene->addItem(secondHand);
    rscene->addItem(secondHandBase);
    rscene->addItem(secondRing);

    roomclockMode = 0;
    showSHand     = 1;
}


ThemeClockWidget::~ThemeClockWidget() {
    delete nscene;
    delete rscene;
}


void ThemeClockWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    this->fitInView(focus, Qt::KeepAspectRatio);
    this->centerOn(focus);
}


void ThemeClockWidget::actPie() {
    setScene(nscene);
    
    if(time < maxtime) bg->setSpanAngle(-round(((double)time*360*16)/(double)maxtime));
    else               bg->setSpanAngle(360*16);

    if(time < maxtime*3/4) bg->setBrush(QBrush(QColor(50,200,30)));
    else                   bg->setBrush(QBrush(QColor(255,190,30)));    


    if(time > maxtime && time < 2*maxtime) mg->setSpanAngle(-round((((double)time-maxtime)*360*16)/(double)maxtime));
    else if(time >= 2*maxtime)             mg->setSpanAngle(360*16);
    else                                   mg->setSpanAngle(0);
    
    if(time > 2*maxtime) fg->setSpanAngle(-round((((double)time-2*(double)maxtime)*360*16)/(double)maxtime));
    else                 fg->setSpanAngle(0);
}


void ThemeClockWidget::actRoomclock() {
    setScene(rscene);

    QTime now = QTime::currentTime();

    double hourHandPos;
    double minuteHandPos;
    double secondHandPos;


    switch (roomclockMode*showSHand) {
        default: // Swiss
            if(now.second() < 58) secondHandPos = 180.0 + 6.0*((int) ((now.second()*1000 + now.msec())*60/58)/1000
                                                               + 0.5-0.5*cos((((now.second()*1000 + now.msec())*60/58)%1000)/1000.0*3.14159));
            else                  secondHandPos = 180.0;

            if(now.second() == 59 && now.msec() > 849) minuteHandPos = 180.0 + now.minute()*6.0 + 6.0*(0.5-0.5*cos((now.msec()-850)/150.0*3.14159));
            else                                       minuteHandPos = 180.0 + now.minute()*6.0;

            hourHandPos = 180.0 + 30.0 * (now.hour() + (minuteHandPos-180.0)/360.0);

            break;

        case 1: // smooth
            secondHandPos = 180.0 + now.second()*6.0 + 6.0*(0.5-0.5*cos(now.msec()/1000.0*3.14159));

            if(now.second() == 59) minuteHandPos = 180.0 + now.minute()*6.0 + 6.0*(0.5-0.5*cos(now.msec()/1000.0*3.14159));
            else                   minuteHandPos = 180.0 + now.minute()*6.0;

            hourHandPos = 180.0 + 30.0 * (now.hour() + (minuteHandPos-180.0)/360.0);

            break;

        case 2: // sharp
            if(now.msec() < 899) secondHandPos = 180.0 + now.second()*6.0;
            else                 secondHandPos = 180.0 + now.second()*6.0 + 6.0*(0.5-0.5*cos((now.msec()-900)/100.0*3.14159));

            minuteHandPos = 180.0 + now.minute()*6.0 + ((secondHandPos-180.0)/360.0)*6.0;

            hourHandPos = 180.0 + 30.0 * (now.hour() + (minuteHandPos-180.0)/360.0);

            break;
    }


    hourHand->setRotation(hourHandPos);
    minuteHand->setRotation(minuteHandPos);
    secondHand->setRotation(secondHandPos);
    secondHandBase->setRotation(secondHandPos);
    secondRing->setRotation(secondHandPos);
}


void ThemeClockWidget::act() {
    if(roomclock) actRoomclock();
    else          actPie();
}


void ThemeClockWidget::setMaximumTime(int ms)       { if(ms > 0) maxtime = ms; }
void ThemeClockWidget::setElapsedTime(int ms)       { time = ms; }
void ThemeClockWidget::setRoomclock(bool roomclock) { this->roomclock = roomclock; }


int  ThemeClockWidget::getElapsedTime() { return time; }
int  ThemeClockWidget::getMaximumTime() { return maxtime; }
bool ThemeClockWidget::isRoomclock()    { return roomclock; }


void ThemeClockWidget::setRoomclockMode(int mode) { roomclockMode = mode; }

void ThemeClockWidget::showSecondHand(bool show) {
    secondHand->setVisible(show);
    secondHandBase->setVisible(show);
    secondRing->setVisible(show);
    showSHand = show? 1 : -1;
    actRoomclock();
}

