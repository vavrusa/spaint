#include <QPainter>
#include "canvasview.h"

CanvasView::CanvasView(Canvas* canvas, QWidget* parent)
      : QGraphicsView(canvas, parent), mCanvas(canvas)
{
   setFrameStyle(QFrame::NoFrame);
   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
   setSceneRect(0, 0, 640, 480);
   show();
}

#include "canvasview.moc"
