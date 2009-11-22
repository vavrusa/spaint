#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsScene>
class CanvasMgr;
class QPainter;
class QGraphicsSceneMouseEvent;

class Canvas : public QGraphicsScene
{
   Q_OBJECT
   Q_PROPERTY(QString name READ name WRITE setName)

   public:
   Canvas(const QString& name = "Canvas", CanvasMgr* parent = 0);

   /** Return canvas name.
     * \return canvas name
     */
   const QString& name() { return mName; }
   /** Set canvas name to given value.
     * \param name new name
     */
   void setName(const QString& name) {
      mName = name;
   }

   /// State tracking
   enum State {
      Idle    = 0x00, // Not interacting
      Drawing = 0x01, // Drawing path
      Gesture = 0x02  // Drawing mouse gesture
   };

   /** Return default canvas size.
       \return canvas size
     */
   static QSize defaultSizeHint() {
      return QSize(640, 480);
   }

   protected:
   void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
   void mousePressEvent(QGraphicsSceneMouseEvent* e);
   void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);

   private:
   int mState;
   QGraphicsPathItem* mGlyph;
   QString mName;

};

#endif // CANVAS_H
