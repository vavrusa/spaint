#include <QPainter>
#include "canvasview.h"

CanvasView::CanvasView(Canvas* canvas, QWidget* parent)
      : QGraphicsView(canvas, parent), mCanvas(canvas)
{
   // Disable frame and scrollbars
   setFrameStyle(QFrame::NoFrame);
   setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

   // TODO: Antialiasing is too CPU intensive, investigate
   //setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
   setSceneRect(canvas->sceneRect());
   setCacheMode(CacheBackground);
   show();
}

#include "canvasview.moc"
