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
#include "colorsicon.h"
#include "numbericon.h"

/** Private data. */
class Overlay::Private
{
   public:
   Private() {
      tool = Canvas::NoTool;
   }

   ColorsIcon* colors;
   Canvas::Tool tool;
   QMap<Canvas::Tool, GraphicsIcon*> icons;
   QGraphicsLinearLayout* layout;
};

Overlay::Overlay(QGraphicsWidget *parent) :
    QGraphicsWidget(parent), d(new Private)
{
   resize(0, 64);

   // Create layout
   d->layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
   setLayout(d->layout);

   // Create buttons
   d->icons.clear();
   d->icons.insert(Canvas::Pen,    new GraphicsIcon(QPixmap(":icons/48x48/draw-pen.png"), this));
   d->icons.insert(Canvas::Eraser, new GraphicsIcon(QPixmap(":icons/48x48/draw-eraser.png"), this));
   d->icons.insert(Canvas::Transform, new GraphicsIcon(QPixmap(":icons/48x48/transform.png"), this));
   d->icons.insert(Canvas::Picker, new GraphicsIcon(QPixmap(":icons/48x48/color-picker.png"), this));

   // Append buttons to layout
   QList<GraphicsIcon*> list = d->icons.values();
   foreach(GraphicsIcon* icon, list) {
      d->layout->addItem(icon);
      connect(icon, SIGNAL(selected(GraphicsIcon*)), this, SLOT(selectIcon(GraphicsIcon*)));
   }

   // Append number icon
   NumberIcon* thickness = new NumberIcon(this);
   d->layout->addItem(thickness);
   connect(thickness, SIGNAL(numberShifted(int)),
           this,      SLOT(changeThickness(int)));
   connect(this,      SIGNAL(thicknessChanged(int)),
           thickness, SLOT(setNumber(int)));

   // Append colors icon
   d->colors = new ColorsIcon(this);
   d->layout->addItem(d->colors);
   connect(d->colors, SIGNAL(colorPicked(QPalette::ColorRole,QColor)),
           this,   SLOT(changeColor(QPalette::ColorRole,QColor)));
   connect(this,   SIGNAL(colorChanged(QPalette::ColorRole,QColor)),
           d->colors, SLOT(setColor(QPalette::ColorRole,QColor)));

   // Connect thickness to color
   connect(this,      SIGNAL(colorChanged(QPalette::ColorRole,QColor)),
           thickness, SLOT(setColor(QPalette::ColorRole,QColor)));
}

Overlay::~Overlay()
{
   delete d;
}

Canvas::Tool Overlay::tool()
{
   return d->tool;
}

void Overlay::setTool(Canvas::Tool tool)
{
   selectIcon(d->icons[tool]);
}

void Overlay::selectIcon(GraphicsIcon* icon)
{
   // Find tool
   d->tool = d->icons.key(icon, Canvas::NoTool);
   qDebug("Selected icon, tool %d", d->tool);

   // Update icons
   foreach(GraphicsIcon* i, d->icons) {
      i->setActivated(i == icon);
   }

   emit toolChanged(d->tool);
}

void Overlay::pickColor(QPalette::ColorRole role)
{
   d->colors->pickColor(role);
}

void Overlay::changeColor(QPalette::ColorRole role, const QColor& color)
{
   emit colorChanged(role, color);
}

void Overlay::changeThickness(int num)
{
   emit thicknessChanged(num);
}

#include "overlay.moc"
