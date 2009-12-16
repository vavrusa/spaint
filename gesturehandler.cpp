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
    QMap<gestureType,DirectionList> gestureMap;
    QMap<gestureType,DirectionList> defaultGestureMap;
    QMap<gestureType,typeData> gestureTypeData;
    QVector<Canvas*> activeCanvases;
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
    d->gestureTypeData[Pen]      = typeData(tr("&Pen"),QIcon(":/icons/32x32/draw-pen.png"));
    d->gestureTypeData[Brush]    = typeData(tr("&Brush"),QIcon(":/icons/32x32/draw-pen.png"));
    d->gestureTypeData[Eraser]   = typeData(tr("&Eraser"),QIcon(":/icons/32x32/draw-eraser.png"));
    d->gestureTypeData[Clear]    = typeData(tr("&Clear"),QIcon(":/icons/32x32/canvas-clear.png"));
    d->gestureTypeData[FColor]   = typeData(tr("&FColor"),QIcon(":/icons/32x32/brush-color.png"));
    d->gestureTypeData[BColor]   = typeData(tr("&BColor"),QIcon(":/icons/32x32/brush-color.png"));

    //saving default gestures
    DirectionList dl;
    dl<<Up<<Left;
    d->defaultGestureMap[Pen] = dl;

    dl.clear();
    dl<<Left<<Down;
    d->defaultGestureMap[Brush] = dl;

    //setting up gesture paths from settings/default values
    QSettings set;
    //pen gesture
    if(set.contains("Gestures/Pen"))
        d->gestureMap[Pen] = strToDl(set.value("Gestures/Pen").toString());
    else
        d->gestureMap[Pen] = d->defaultGestureMap[Pen];

    //brush gesture
    if(set.contains("Gestures/Brush"))
        d->gestureMap[Brush] = strToDl(set.value("Gestures/Brush").toString());
    else
        d->gestureMap[Brush] = d->defaultGestureMap[Brush];

    //setting up gestures in recognizer
    QMapIterator<gestureType,DirectionList> i(d->gestureMap);
    while (i.hasNext())
    {
        i.next();
        addGestureDefinition(Definition(i.value(),i.key()));
    }

    emit somethingChanged();

    connect(this, SIGNAL(recognized(int)), this, SLOT(debugGesture(int)));
}

void Handler::uninitializeGestures()
{
    QSettings set;
    //save changed gestures
    if(d->defaultGestureMap[Pen] != d->gestureMap[Pen])
        set.setValue("Gestures/Pen", dlToStr(d->gestureMap[Pen]));

    if(d->defaultGestureMap[Brush] != d->gestureMap[Brush])
        set.setValue("Gestures/Brush", dlToStr(d->gestureMap[Brush]));
}

void Handler::debugGesture(int code)
{
   qDebug() << "Gesture recognized: " << code;
}

bool Handler::observe(CanvasMgr* cm)
{
    connect(cm, SIGNAL(canvasCreated(Canvas*)), this, SLOT(handleCanvas(Canvas*)));
    connect(cm, SIGNAL(canvasRemoved(Canvas*)), this, SLOT(letCanvasGo(Canvas*)));
    return true;
}

DirectionList Handler::getGesture(gestureType type)
{
    return d->gestureMap[type];
}

void Handler::setGesture(gestureType type, DirectionList dl)
{
    clearGestureDefinitions();

    d->gestureMap[type] = dl;

    //redelcaring of gestures
    QMapIterator<gestureType,DirectionList> i(d->gestureMap);
    while (i.hasNext())
    {
        i.next();
        addGestureDefinition(Definition(i.value(),i.key()));
    }

    emit somethingChanged();
}

void Handler::resetGesture(gestureType type)
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

typeData& Handler::getTypeData(gestureType type)
{
   return d->gestureTypeData[type];
}

unsigned Handler::getTypeCount()
{
   return d->gestureTypeData.count();
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
    d->activeCanvases.push_back(cnvs);
}

void Handler::letCanvasGo(Canvas* cnvs)
{
    d->activeCanvases.remove(d->activeCanvases.indexOf(cnvs));
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
