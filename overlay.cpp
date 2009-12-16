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

#include <QGraphicsLinearLayout>
#include <QPainter>
#include "overlay.h"
#include "graphicsicon.h"

/** Private data. */
class Overlay::Private
{
   public:
   Private() {
   }

   QGraphicsLinearLayout* layout;
};

Overlay::Overlay(QGraphicsWidget *parent) :
    QGraphicsWidget(parent), d(new Private)
{
   resize(0, 64);

   // Create layout
   d->layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
   setLayout(d->layout);
   d->layout->addItem(new GraphicsIcon(QPixmap(":icons/48x48/draw-brush.png"), this));
   d->layout->addItem(new GraphicsIcon(QPixmap(":icons/48x48/fill-color.png"), this));
   d->layout->addItem(new GraphicsIcon(QPixmap(":icons/48x48/draw-eraser.png"), this));
   d->layout->addItem(new GraphicsIcon(QPixmap(":icons/48x48/color-picker.png"), this));
}

Overlay::~Overlay()
{
   delete d;
}

void Overlay::paint(QPainter* p, const QStyleOptionGraphicsItem* opt, QWidget* w)
{
   // Paint background
   QGraphicsWidget::paint(p, opt, w);
}

#include "overlay.moc"
