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

    //Checks, if defined line is ok for building gesture
    bool isBlock(QPoint a, QPoint b);

    enum State{ Idle, Drawing};

public slots:
    void show();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void eraseCurrentGestures();        //erase current drawing of gesture
    void currentGestureChanged();       //drawing has changed
    void editCurrentGesture();          //save current gesture to handler

private:
    class Private;
    Private* d;
    void defineLayout();
    void makeConnections();





};

}
#endif // GESTUREEDITOR_H
