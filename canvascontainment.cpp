#include <QMap>
#include <QWheelEvent>
#include "canvascontainment.h"
#include "canvasview.h"
#include "canvas.h"

struct CanvasContainment::Private {
   QMap<Canvas*,CanvasView*> map;
};

CanvasContainment::CanvasContainment(QWidget *parent)
   : QStackedWidget(parent), d(new Private)
{
}

CanvasContainment::~CanvasContainment()
{
   delete d;
}

void CanvasContainment::addCanvas(Canvas* c)
{
   CanvasView* view = new CanvasView(c, this);
   d->map.insert(c, view);
   addWidget(view);
}

void CanvasContainment::removeCanvas(Canvas* c)
{
   CanvasView* view = d->map.take(c);
   if(view == 0)
      return;

   removeWidget(view);
   view->deleteLater();
}

void CanvasContainment::wheelEvent(QWheelEvent* e)
{
   QWidget* w = widget(currentIndex() + 1);
   if(w == 0) { // End of list
      w = widget(0); // Reset
   }

   // Check widget
   if(w != 0) {
      setCurrentWidget(w);
      return;
   }

   QStackedWidget::wheelEvent(e);
}

#include "canvascontainment.moc"
