#ifndef GESTUREEDITOR_H
#define GESTUREEDITOR_H

#include "QWidget"
#include "gesturehandler.h"


namespace Gesture{


class GestureEditor : public QWidget
{
    Q_OBJECT

public:
    GestureEditor(Handler* parent);
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
    void makeConnections();





};

}
#endif // GESTUREEDITOR_H
