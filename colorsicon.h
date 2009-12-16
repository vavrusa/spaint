/***************************************************************************
 *   Copyright (C) 2009 Marek Vavrusa <marek@vavrusa.com>                  *
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

#ifndef COLORSICON_HPP
#define COLORSICON_HPP
#include "graphicsicon.h"

class ColorsIcon : public GraphicsIcon
{
   Q_OBJECT

   public:

   /** Explicit constructor. */
   explicit ColorsIcon(QGraphicsItem *parent = 0);

   /** Paint event. */
   void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w = 0);

   public slots:

   /** Set color. */
   void setColor(QPalette::ColorRole role, const QColor& color);

   /** Color picker. */
   void pickColor(QPalette::ColorRole role);

   signals:

   /** Color changed. */
   void colorPicked(QPalette::ColorRole, QColor);

   protected:

   /** Mouse press. */
   void mousePressEvent(QGraphicsSceneMouseEvent* e);

   private:
   QColor mPen, mBrush;
};

#endif
