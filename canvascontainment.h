#ifndef CANVASCONTAINMENT_H
#define CANVASCONTAINMENT_H

#include <QGraphicsScene>
class QGraphicsSceneWheelEvent;
class QGraphicsView;
class QPainter;
class Canvas;

class CanvasContainment : public QGraphicsScene
{
   Q_OBJECT

   public:
   CanvasContainment(QWidget *parent = 0);
   ~CanvasContainment();

   QGraphicsView* view();

   // State tracking
   enum State {
      Focused = 0x00,
      Exposed = 0x01
   };

   signals:

   public slots:
   void addCanvas(Canvas* c);
   void removeCanvas(Canvas* c);

   protected:
   void drawBackground(QPainter* p, const QRectF& re);
   void wheelEvent(QGraphicsSceneWheelEvent* e);

   private:
   struct Private;
   Private* d;

};

#endif // CANVASCONTAINMENT_H
