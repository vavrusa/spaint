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

   signals:

   public slots:
   void addCanvas(Canvas* c);
   void removeCanvas(Canvas* c = 0);
   void focusToCanvas(Canvas* c);
   void clearCanvas(Canvas* c = 0);
   void renderCanvas(QIODevice& device, Canvas* c = 0);

   protected:
   void drawBackground(QPainter* p, const QRectF& re);
   void wheelEvent(QGraphicsSceneWheelEvent* e);

   private slots:
   void updateLayout(const QRectF& newRect);

   private:
   struct Private;
   Private* d;

};

#endif // CANVASCONTAINMENT_H
