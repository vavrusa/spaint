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

#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QPainter>
#include "numbericon.h"

NumberIcon::NumberIcon(QGraphicsItem *parent)
   : GraphicsIcon(QPixmap(), parent), mNum(1), mDiff(0), mColor(Qt::black)
{
   resize(48, 48);
   setAcceptedMouseButtons(Qt::LeftButton);
}

void NumberIcon::paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w)
{
   // Paint hover effect
   GraphicsIcon::paint(p, opt, w);

   // Calculate draw rect
   int height = opt->fontMetrics.height();
   QRectF re(boundingRect());
   QPointF c(re.center());
   re.setHeight(height * 1.2);
   re.moveTop(c.y() - re.height());
   p->save();

   // Paint text
   QFont font(p->font());
   font.setBold(true);
   p->setFont(font);
   p->drawText(re, QString::number(mNum) + " px");
   re.moveTop(c.y() + re.height() * 0.5);

   // Paint line
   QPen pen(p->pen());
   pen.setColor(mColor);
   pen.setWidth(mNum + mDiff);
   if(pen.width() > re.height())
      pen.setWidth(re.height() - 2);

   p->setPen(pen);
   p->drawLine(re.topLeft(), re.topRight());
   p->restore();
}

void NumberIcon::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
   mDiff = 0;
}

void NumberIcon::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
   mDiff = 0.1 * (e->buttonDownPos(Qt::LeftButton).y() - e->pos().y());
   if(mNum + mDiff < 1)
      mDiff = -mNum + 1;
   update();
}

void NumberIcon::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
   emit numberShifted(mNum + mDiff);
   mDiff = 0;
}

#include "numbericon.moc"
