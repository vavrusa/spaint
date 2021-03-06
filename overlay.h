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

#ifndef OVERLAY_HPP
#define OVERLAY_HPP
#include <QGraphicsWidget>
#include "canvas.h"
class GraphicsIcon;

class Overlay : public QGraphicsWidget
{
   Q_OBJECT

   public:

   /** Initialize overlay. */
   Overlay(QGraphicsWidget* parent = 0);
   ~Overlay();

   /** Return tool. */
   Canvas::Tool tool();

   /** Set tool. */
   void setTool(Canvas::Tool tool);

   signals:

   /** Color changed. */
   void colorChanged(QPalette::ColorRole, QColor);

   /** Thickness changed. */
   void thicknessChanged(int);

   /** Tool changed. */
   void toolChanged(Canvas::Tool);

   public slots:

   /** Pick color. */
   void pickColor(QPalette::ColorRole role);

   /** Change color. */
   void changeColor(QPalette::ColorRole role, const QColor& color);

   /** Change thickness. */
   void changeThickness(int num);

   /** Select icon. */
   void selectIcon(GraphicsIcon* icon);

   private:
   struct Private;
   Private* d;
};

#endif
