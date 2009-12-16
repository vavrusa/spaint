#ifndef GESTUREEDITORCANVAS_H
#define GESTUREEDITORCANVAS_H

#include <QWidget>
#include <QMouseEvent>
#include "gestureeditor.h"

namespace Gesture{

class GestureEditorCanvas : public QWidget
{
    Q_OBJECT
public:

    GestureEditorCanvas(GestureEditor* parent = 0);
    ~GestureEditorCanvas();

    enum State{ Idle, Drawing};

    //Checks, if defined line is ok for building gesture
    bool isBlock(QPoint a, QPoint b);
    DirectionList getDirections();
\
public slots:
    void setDirections(DirectionList dl);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private:
    class Private;
    Private* d;
};

}

#endif // GESTUREEDITORCANVAS_H
