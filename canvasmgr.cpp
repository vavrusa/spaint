#include <QList>
#include "canvasmgr.h"

struct CanvasMgr::Private {
   QList<Canvas*> list;
};

CanvasMgr::CanvasMgr(QObject* parent)
      : QObject(parent), d(new Private)
{
}

CanvasMgr::~CanvasMgr()
{
   QList<Canvas*>::iterator it;
   for(it = d->list.begin(); it < d->list.end(); ++it)
      remove(*it);

   delete d;
}

Canvas* CanvasMgr::create(const QString& name)
{
   // TODO
   Canvas* c = new Canvas(name, this);

   // Emit signal
   emit canvasCreated(c);
   return c;
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
