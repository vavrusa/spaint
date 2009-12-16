#ifndef GESTUREEDITOR_H
#define GESTUREEDITOR_H

#include <QDialog>
#include "gesturehandler.h"


namespace Gesture {


class GestureEditor : public QDialog
{
   Q_OBJECT

   public:
   GestureEditor(Handler* h, QWidget* parent = 0);
   ~GestureEditor();

   public slots:
   void show();

   private slots:
   void eraseCurrentGestures();        //erase current drawing of gesture
   void currentGestureChanged();       //drawing has changed
   void editCurrentGesture();          //save current gesture to handler
   void resetCurrentGesture();         //set current gesture to default value

   private:
   class Private;
   Private* d;

   void defineLayout();
};

}
#endif
