#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
class CanvasMgr;
class QGraphicsSceneMouseEvent;

class Canvas : public QGraphicsScene
{
   Q_OBJECT

   public:
   Canvas(CanvasMgr* parent = 0);

   // State tracking
   enum State {
      Idle    = 0x00,
      Drawing = 0x01,
   };

   protected:
   void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
   void mousePressEvent(QGraphicsSceneMouseEvent* e);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

   private:
   int mState;
   QGraphicsPathItem* mGlyph;

};

#endif // CANVAS_H
