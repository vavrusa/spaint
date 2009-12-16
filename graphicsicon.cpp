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
      isActivated = false;
   }

   int timer;
   float dist;
   bool isActivated;
   QPixmap pixmap;
};

GraphicsIcon::GraphicsIcon(const QPixmap& pixmap, QGraphicsItem* parent)
   : QGraphicsWidget(parent), d(new Private)
{
   d->pixmap = pixmap;
   resize(pixmap.size());
   setAcceptHoverEvents(true);
   setAcceptedMouseButtons(Qt::LeftButton);
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
   if(d->dist > 0.0 || d->isActivated) {
      QColor color(255, 255, 255, 192);
      float radius = (re.height() * 0.75) * d->dist;
      if(d->isActivated) {
         radius = (re.height() * 0.75) * 1.0;
         color = QColor(168, 255, 255, 224);
      }

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

void GraphicsIcon::setActivated(bool value) {
   d->isActivated = value;
   setAcceptHoverEvents(!value); // Activated does not accept hover
   update();
}

void GraphicsIcon::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
   if(e->button() == Qt::LeftButton)
      emit selected(this);
}

void GraphicsIcon::hoverMoveEvent(QGraphicsSceneHoverEvent* e)
{
   QRectF re(boundingRect());
   d->dist = 1.0 - fabs(e->pos().x() - re.center().x()) / re.width() * 0.5;
   update();
}

void GraphicsIcon::hoverEnterEvent(QGraphicsSceneHoverEvent* e)
{
   if(d->timer != -1) {
      killTimer(d->timer);
   }
}

void GraphicsIcon::hoverLeaveEvent(QGraphicsSceneHoverEvent* e)
{
   d->timer = startTimer(40);
}

#include "graphicsicon.moc"
