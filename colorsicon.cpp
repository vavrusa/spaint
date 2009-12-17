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
#include <QColorDialog>
#include <QLayout>
#include <QDialogButtonBox>
#include <QPainter>
#include "colorsicon.h"

ColorsIcon::ColorsIcon(QGraphicsItem *parent)
   : GraphicsIcon(QPixmap(), parent), mPen(Qt::black), mBrush(255,255,255,0)
{
   resize(48, 48);
}

void ColorsIcon::paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w)
{
   // Draw background
   GraphicsIcon::paint(p, opt, w);

   // Divide rect in two halves
   QRectF re(boundingRect());
   re.setSize(QSizeF(48, 48));
   re.moveTop(re.top() + 0.5 * (boundingRect().height() - re.height()));
   re.moveLeft(re.left() + 0.5 * (boundingRect().width() - re.width()));
   re.setWidth(re.width() * 0.5);

   // Foreground
   p->fillRect(re, mPen);

   // Backgorund
   re.moveLeft(re.left() + re.width());
   p->fillRect(re, mBrush);

   // Draw border
   re.moveLeft(re.left() - re.width());
   re.setWidth(re.width() * 2.0);
   p->setPen(Qt::black);
   p->setBrush(Qt::NoBrush);
   p->drawRect(re);
}

void ColorsIcon::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
   // Get first cell
   float divider = boundingRect().width() * 0.5;

   // Match mouse
   if(e->pos().x() < divider) {
      pickColor(QPalette::Foreground);
   }
   else {
      pickColor(QPalette::Background);
   }
}

void ColorsIcon::pickColor(QPalette::ColorRole role)
{
   // TODO: QColorDialog is bugged. Corrupts alpha when no adjustment occurs.
   QColorDialog dlg;
   dlg.setOption(QColorDialog::DontUseNativeDialog);
   dlg.setOption(QColorDialog::ShowAlphaChannel);
   dlg.setCurrentColor(role == QPalette::Foreground ? mPen : mBrush);
   dlg.setWindowTitle(role == QPalette::Foreground ?
                      tr("Select pen color") :
                      tr("Select brush color"));

   // Execute dialog
   if(dlg.exec() == QDialog::Accepted) {

      // Emit color change
      emit colorPicked(role, dlg.currentColor());
   }
}

void ColorsIcon::setColor(QPalette::ColorRole role, const QColor& color) {

   if(role == QPalette::Background)
      mBrush = color;
   else
      mPen = color;

   // Update
   update();
}

#include "colorsicon.moc"
