#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <QGraphicsView>
#include "canvas.h"
class QPainter;

class CanvasView : public QGraphicsView
{
   Q_OBJECT

   public:
   CanvasView(Canvas* canvas, QWidget* parent = 0);

   private:
   Canvas* mCanvas;

};

#endif // CANVASVIEW_H
