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

#ifndef GESTUREHANDLER_H
#define GESTUREHANDLER_H

#include <QObject>
#include <QPainterPath>
#include <QPair>
#include <QList>
#include <QString>
#include <QIcon>
#include "canvas.h"
#include "canvasmgr.h"
#include "mousegesturerecognizer.h"

namespace Gesture
{

enum Type {
   Pen,
   Brush,
   Eraser,
   Clear,
   FColor,
   BColor,
};

typedef QPair<QString, QIcon> Info;

class Handler : public MouseGestureRecognizer
{
   Q_OBJECT

   public:
   Handler(QObject* parent = 0);
   ~Handler();
   bool observe(CanvasMgr* cm);
   DirectionList getGesture(Type type);              //gets direction list of gesture
   void setGesture(Type type, DirectionList dl);     //changes directions of gesture
   void resetGesture(Type type);                      //set gestures directions to default

   //converting string <-> DirectionList
   DirectionList strToDl (QString str);
   QString dlToStr (DirectionList dl);
   const QMap<Type,Info>& getTypes();            //returns label and icons mapped to type

   public slots:
   bool start();              //start/stop handling gestures
   bool stop();
   void handleGesture(QPainterPath gesture);   //procedure which takes care of incoming gesture
   void handleCanvas(Canvas* cnvs);            //connecting with newly created canvas
   void letCanvasGo(Canvas* cnvs);             //disconnecting from destroyed canvas

   protected:
   void initializeGestures();               //sets all gestures either from settings or from default values
   void uninitializeGestures();             //saves all changed gestures to settings

   signals:
   void somethingChanged();

   private:
   class Private;
   Private* d;
};

}
#endif // GESTUREHANDLER_H
