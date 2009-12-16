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

#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QCursor>
#include <cmath>
#include "graphicsicon.h"

/** Private data. */
class GraphicsIcon::Private
{
   public:
   Private() {
      timer = -1;
      dist = 0.0;
   }

   int timer;
   float dist;
   QPixmap pixmap;
};

GraphicsIcon::GraphicsIcon(const QPixmap& pixmap, QGraphicsItem* parent)
   : QGraphicsWidget(parent), d(new Private)
{
   d->pixmap = pixmap;
   resize(pixmap.size());
   setAcceptHoverEvents(true);
   setPreferredSize(pixmap.size());
}

GraphicsIcon::~GraphicsIcon()
{
   delete d;
}

void GraphicsIcon::paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w)
{
   // Draw background
   QRectF re(boundingRect());
   if(d->dist > 0.0) {
      float radius = (re.height() * 0.75) * d->dist;
      QColor color(255, 255, 255, 192);
      QRadialGradient grad(re.center(), radius);
      grad.setColorAt(0.0, color);
      color.setAlpha(0);
      grad.setColorAt(1.0, color);
      p->save();
      p->setPen(Qt::NoPen);
      p->setBrush(QBrush(grad));
      p->drawEllipse(re.center(), radius, radius);
      p->restore();
   }

   // Draw icon
   QPointF center(re.center());
   center.setX(center.x() - d->pixmap.width() * 0.5);
   center.setY(center.y() - d->pixmap.height() * 0.5);
   p->drawPixmap(center, d->pixmap);
}

void GraphicsIcon::timerEvent(QTimerEvent* e)
{
   if(d->dist <= 0.0) {
      killTimer(d->timer);
      d->dist = 0.0;
      d->timer = -1;
   }
   else {
      d->dist -= 0.1;
   }

   // Update
   update();
}

void GraphicsIcon::hoverMoveEvent(QGraphicsSceneHoverEvent* e)
{
   QRectF re(boundingRect());
   d->dist = 1.0 - fabs(e->pos().x() - re.center().x()) / re.width() * 0.5;
   update();
}

void GraphicsIcon::hoverEnterEvent(QGraphicsSceneHoverEvent* e)
{
   qDebug("enter");
   if(d->timer != -1) {
      killTimer(d->timer);
   }
}

void GraphicsIcon::hoverLeaveEvent(QGraphicsSceneHoverEvent* e)
{
   qDebug("leave");
   d->timer = startTimer(40);
}

#include "graphicsicon.moc"
