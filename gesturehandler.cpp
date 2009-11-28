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
#include <QDebug>
#include <QList>

using namespace Gesture;

class Handler::Private
{
   public:
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
   delete d;
}

void Handler::initializeGestures()
{

    DirectionList dl;
    dl<<Up<<Left;

    addGestureDefinition(Gesture::Definition(dl, Gesture::Pen));
    connect(this, SIGNAL(recognized(int)), this, SLOT(debugGesture(int)));
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
