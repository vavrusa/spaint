#include "canvasmgr.h"

CanvasMgr::CanvasMgr(QObject* parent)
      : QObject(parent)
{
}

CanvasMgr::~CanvasMgr()
{
}

Canvas* CanvasMgr::create(const QString& name)
{
   // TODO
   Canvas* c = new Canvas(this);

   // Emit signal
   emit canvasCreated(c);
}

bool CanvasMgr::remove(Canvas* canvas)
{
   // Emit signal
   emit canvasRemoved(canvas);

   // TODO
   canvas->deleteLater();
   return true;
}

#include "canvasmgr.moc"
