#ifndef CANVASMGR_H
#define CANVASMGR_H

#include <QString>
#include "canvas.h"

class CanvasMgr : public QObject
{
   Q_OBJECT

   public:
   CanvasMgr(QObject* parent = 0);
   ~CanvasMgr();

   public slots:
   Canvas* create(const QString& name = QString());
   bool remove(Canvas* canvas);

   signals:
   void canvasCreated(Canvas* canvas);
   void canvasRemoved(Canvas* canvas);

};

#endif // CANVASMGR_H
