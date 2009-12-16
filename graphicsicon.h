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

#ifndef GRAPHICSICON_HPP
#define GRAPHICSICON_HPP
#include <QGraphicsWidget>
#include <QPixmap>

class GraphicsIcon : public QGraphicsWidget
{
   Q_OBJECT

   public:

   /** Initializes graphics icon with pixmap. */
   GraphicsIcon(const QPixmap& pixmap, QGraphicsItem* parent = 0);
   ~GraphicsIcon();

   /** Paint event. */
   void paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w = 0);

   /** Activate. */
   void setActivated(bool value);

   signals:

   /** Emit activated signal. */
   void selected(GraphicsIcon* icon);

   protected:

   /** Hover events. */
   void mousePressEvent(QGraphicsSceneMouseEvent* e);
   void hoverEnterEvent(QGraphicsSceneHoverEvent* e);
   void hoverLeaveEvent(QGraphicsSceneHoverEvent* e);
   void hoverMoveEvent(QGraphicsSceneHoverEvent* e);
   void timerEvent(QTimerEvent* e);

   private:
   struct Private;
   Private* d;
};

#endif
