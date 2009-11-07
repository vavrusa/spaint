#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
class CanvasMgr;

class Canvas : public QGraphicsScene
{
   Q_OBJECT

   public:
   Canvas(CanvasMgr* parent = 0);
};

#endif // CANVAS_H
