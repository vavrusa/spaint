/***************************************************************************
 *   Copyright (C) 2010 Brno University of Technology,                     *
 *   Faculty of Information Technology                                     *
 *   Author(s): Marek Vantuch    <xvantu00 AT stud.fit.vutbr.cz>           *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "gesturehandler.h"
#include <QString>
#include <QDebug>
#include <QList>
#include <QSettings>

using namespace Gesture;

class Handler::Private
{
   public:
    QMap<Type,DirectionList> gestureMap;
    QMap<Type,DirectionList> defaultGestureMap;
    QMap<Type,Info> gestureTypeData;
    bool isRunning;
};

Handler::Handler(QObject* parent)
   : MouseGestureRecognizer(parent), d(new Private)
{
    d->isRunning = false;

    initializeGestures();
}

Handler::~Handler()
{
    uninitializeGestures();
    delete d;
}

void Handler::initializeGestures()
{

    //inicializing text and icon for all of the gesture types, which can be set
    d->gestureTypeData[Pen]      = Info(tr("Select pen"),QIcon(":/icons/32x32/draw-pen.png"));
    d->gestureTypeData[Eraser]   = Info(tr("Select eraser"),QIcon(":/icons/32x32/draw-eraser.png"));
    d->gestureTypeData[Clear]    = Info(tr("Clear canvas"),QIcon(":/icons/32x32/canvas-clear.png"));
    d->gestureTypeData[FColor]   = Info(tr("Pen color"),QIcon(":/icons/32x32/pen-color.png"));
    d->gestureTypeData[BColor]   = Info(tr("Background color"),QIcon(":/icons/32x32/brush-color.png"));

    //saving default gestures
    d->defaultGestureMap[Pen] = DirectionList() << Up << Right;
    d->defaultGestureMap[Eraser] = DirectionList() << Up << Left;
    d->defaultGestureMap[Clear] = DirectionList() << Down;
    d->defaultGestureMap[FColor] = DirectionList() << Up << Right << Down << Left;
    d->defaultGestureMap[BColor] = DirectionList() << Down << Right << Up << Left;

    //setting up gesture paths from settings/default values
    QSettings set;

    //pen gesture
    d->gestureMap = d->defaultGestureMap;
    if(set.contains("Gestures/Pen"))
        d->gestureMap[Pen] = strToDl(set.value("Gestures/Pen").toString());
    if(set.contains("Gestures/Eraser"))
        d->gestureMap[Eraser] = strToDl(set.value("Gestures/Eraser").toString());
    if(set.contains("Gestures/Clear"))
        d->gestureMap[Clear] = strToDl(set.value("Gestures/Clear").toString());
    if(set.contains("Gestures/FColor"))
        d->gestureMap[FColor] = strToDl(set.value("Gestures/FColor").toString());
    if(set.contains("Gestures/BColor"))
        d->gestureMap[BColor] = strToDl(set.value("Gestures/BColor").toString());


    //setting up gestures in recognizer
    QMapIterator<Type,DirectionList> i(d->gestureMap);
    while (i.hasNext())
    {
        i.next();
        addGestureDefinition(Definition(i.value(),i.key()));
    }

    emit somethingChanged();
}

void Handler::uninitializeGestures()
{
    QSettings set;
    set.setValue("Gestures/Pen",    dlToStr(d->gestureMap[Pen]));
    set.setValue("Gestures/Eraser", dlToStr(d->gestureMap[Eraser]));
    set.setValue("Gestures/Clear",  dlToStr(d->gestureMap[Clear]));
    set.setValue("Gestures/FColor", dlToStr(d->gestureMap[FColor]));
    set.setValue("Gestures/BColor", dlToStr(d->gestureMap[BColor]));
    set.sync();
}

bool Handler::observe(CanvasMgr* cm)
{
    connect(cm, SIGNAL(canvasCreated(Canvas*)), this, SLOT(handleCanvas(Canvas*)));
    connect(cm, SIGNAL(canvasRemoved(Canvas*)), this, SLOT(letCanvasGo(Canvas*)));
    return true;
}

DirectionList Handler::getGesture(Type type)
{
    return d->gestureMap[type];
}

void Handler::setGesture(Type type, DirectionList dl)
{
    clearGestureDefinitions();

    d->gestureMap[type] = dl;

    //redelcaring of gestures
    QMapIterator<Type,DirectionList> i(d->gestureMap);
    while (i.hasNext())
    {
        i.next();
        addGestureDefinition(Definition(i.value(),i.key()));
    }

    emit somethingChanged();
}

void Handler::resetGesture(Type type)
{
    d->gestureMap[type] = d->defaultGestureMap[type];
    emit somethingChanged();
}

DirectionList Handler::strToDl(QString str)
{
    DirectionList dl;
    for(int i = 0; i < str.size(); i++)
    {
        switch(str.toLatin1()[i])
        {
        case 'U':
            dl<<Up;
            break;
        case 'D':
            dl<<Down;
            break;
        case 'L':
            dl<<Left;
            break;
        case 'R':
            dl<<Right;
            break;
        }
    }
    return dl;
}

QString Handler::dlToStr(DirectionList dl)
{
    QString str;
    for(int i = 0; i < dl.size(); i++)
    {
        switch(dl.at(i))
        {
        case Up:
            str.append('U');
            break;
        case Down:
            str.append('D');
            break;
        case Left:
            str.append('L');
            break;
        case Right:
            str.append('R');
            break;
        }
    }
    return str;

}

const QMap<Type,Info>& Handler::getTypes()
{
   return d->gestureTypeData;
}

bool Handler::start()
{
    return d->isRunning = true;
}

bool Handler::stop()
{
    return d->isRunning = false;
}

void Handler::handleCanvas(Canvas *cnvs)
{
    connect(cnvs, SIGNAL(gestureCreated(QPainterPath)), this, SLOT(handleGesture(QPainterPath)));
}

void Handler::letCanvasGo(Canvas* cnvs)
{
    disconnect(cnvs, SIGNAL(gestureCreated(QPainterPath)), this, SLOT(handleGesture(QPainterPath)));
}

void Handler::handleGesture(QPainterPath gesture)
{
    if(d->isRunning)
    {
        qDebug() << "Received gesture: " <<endl;

        QPainterPath::Element elem;
        int x,y;
        for(int i = 0; i < gesture.elementCount(); i++)
        {
            elem = gesture.elementAt(i);
            x = static_cast<int>(elem.x);
            y = static_cast<int>(elem.y);
            //first step
            if(elem.isMoveTo())
            {
                //qDebug()<< "move x: " << elem.x <<"y: " <<elem.y <<" ";

                startGesture(x,y);
            }
            if(elem.isLineTo())
            {
                //qDebug()<< "bod x: " << elem.x <<"y: " <<elem.y <<" ";
                //last item - finish gesture
                if(i==gesture.elementCount()-1)
                {
                    endGesture(x,y);
                }
                else
                    addPoint(x,y);
            }
        }
    }
}

#include "gesturehandler.moc"
