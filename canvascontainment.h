#ifndef CANVASCONTAINMENT_H
#define CANVASCONTAINMENT_H

#include <QStackedWidget>
class QWheelEvent;
class Canvas;

class CanvasContainment : public QStackedWidget
{
   Q_OBJECT

   public:
   CanvasContainment(QWidget *parent = 0);
   ~CanvasContainment();

   signals:

   public slots:
   void addCanvas(Canvas* c);
   void removeCanvas(Canvas* c);

   protected:
   void wheelEvent(QWheelEvent* e);

   private:
   struct Private;
   Private* d;

};

#endif // CANVASCONTAINMENT_H
