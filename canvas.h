/***************************************************************************
 *   Copyright (C) 2010 Brno University of Technology,                     *
 *   Faculty of Information Technology                                     *
 *   Author(s): Marek Vavrusa    <xvavru00 AT stud.fit.vutbr.cz>           *
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

#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
#include <QPalette>
class CanvasMgr;
class QPainter;
class QGraphicsSceneMouseEvent;
class QGraphicsColorizeEffect;

class Canvas : public QGraphicsScene
{
   Q_OBJECT
   Q_PROPERTY(QString name READ name WRITE setName)

   public:

   /// State tracking
   enum State {
      Idle    = 0x00, // Not interacting
      Drawing = 0x01, // Drawing path
      Gesture = 0x02  // Drawing mouse gesture
   };

   /// Tool
   enum Tool {
      NoTool = 0,// Invalid tool
      Pen,       // Pen tool
      Eraser,    // Eraser
      Transform,  // Transform
      Brush,     // Brush
      Picker    // Color picker
   };

   /** Constructor. */
   Canvas(const QString& name = "Canvas", bool imported = false, CanvasMgr* parent = 0);

   /** Return canvas name.
     * \return canvas name
     */
   const QString& name() { return mName; }
   /** Set canvas name to given value.
     * \param name new name
     */
   void setName(const QString& name) {
      mName = name;
   }

   /** Return color. */
   QColor color(QPalette::ColorRole role = QPalette::Foreground) {
      if(role == QPalette::Background)
         return mBrush.color();

      return mPen.color();
   }

   /** Set color. */
   void setColor(QPalette::ColorRole role, const QColor& color) {
      if(role == QPalette::Background)
         mBrush.setColor(color);
      else
         mPen.setColor(color);
   }

   /** Return thickness. */
   int thickness() {
      return mPen.width();
   }

   /** Set thickness. */
   void setThickness(int width) {
      mPen.setWidth(width);
   }

   /** Return tool. */
   Tool tool() {
      return mTool;
   }

   /** Set tool. */
   void setTool(Tool tool) {
      mTool = tool;
   }

   /** Reimplement remove. */
   void removeItem(QGraphicsItem* item) {
      if(item == mHovered)
         mHovered = 0;
      QGraphicsScene::removeItem(item);
   }

   /** Create associated canvas view.
     * \param parent - parent widget for view
     * \return view associated with canvas
     */
   QGraphicsView* createView(QWidget* parent = 0);

   /** Return default canvas size.
       \return canvas size
     */
   static QSize defaultSizeHint() {
      return QSize(640, 480);
   }

   signals:
   /** Signalizing new path on canvas.
     * Signal is emmited only when path is painted non-programatically,
     * e.g. by mouse/touch interface.
     * \param path new path
     */
   void pathCreated(Canvas* canvas, QPainterPath path);
   /** Signalizing new gesture on canvas.
     * \param gesture created gesture
     */
   void gestureCreated(QPainterPath gesture);

   public slots:
   /** Create imported path
     */
   void importPath(QPainterPath path);

   protected:
   void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
   void mousePressEvent(QGraphicsSceneMouseEvent* e);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

   private:
   int mState;
   QGraphicsPathItem* mGlyph;
   QString mName;
   bool mImported;
   QPen mPen;
   QBrush mBrush;
   Tool mTool;
   QGraphicsItem* mHovered;
   QGraphicsColorizeEffect* mEffect;
};

#endif // CANVAS_H
