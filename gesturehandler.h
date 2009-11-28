#ifndef GESTUREHANDLER_H
#define GESTUREHANDLER_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QDebug>
#include "canvas.h"
#include "canvasmgr.h"
#include "mousegesturerecognizer.h"

namespace Gesture
{
    typedef enum{
        Pen,
        Brush,
    } gestureType;



    class GestureHandler : public QObject
    {
        Q_OBJECT

    public:
        GestureHandler();
        bool observe(CanvasMgr* cm);

    public slots:
        bool start();              //start/stop handling gestures
        bool stop();

        void handleGesture(QPainterPath gesture);   //procedure which takes care of incoming gesture
        void handleCanvas(Canvas* cnvs);            //connecting with newly created canvas
        void letCanvasGo(Canvas* cnvs);             //disconnecting from destroyed canvas
        friend class GestureCallbackToSignal;
    signals:
        void emitGesture(gestureType);

    private:
        class Private;
        Private* d;
        void inicializeGestures(void);

    };


    class GestureCallbackToSignal: public Gesture::MouseGestureCallback
    {
    public:
        GestureCallbackToSignal( GestureHandler* gh, gestureType gt)
        {
            ghptr = gh;
            type = gt;
        }

        void callback()
        {
            ghptr->emitGesture(type);
            qDebug()<<"Succesfull gesture:"<<type;
        }
    private:
        GestureHandler* ghptr;
        gestureType type;
    };



}
#endif // GESTUREHANDLER_H
