#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
class CanvasMgr;
class QPainter;
class QGraphicsSceneMouseEvent;

class Canvas : public QGraphicsScene
{
   Q_OBJECT
   Q_PROPERTY(QString name READ name);

   public:
   Canvas(const QString& name = QString(), CanvasMgr* parent = 0);

   // Properties
   const QString& name() { return mName; }

   // State tracking
   enum State {
      Idle    = 0x00,
      Drawing = 0x01,
   };

   protected:
   void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
   void mousePressEvent(QGraphicsSceneMouseEvent* e);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
   void drawBackground(QPainter* p, const QRectF& re);

   private:
   int mState;
   QGraphicsPathItem* mGlyph;
   QString mName;

};

#endif // CANVAS_H
