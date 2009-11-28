#include "gesturehandler.h"
#include <QDebug>
#include <Qlist>

using namespace Gesture;


class GestureHandler::Private
{
public:
    QVector<Canvas*> activeCanvases;
    QList<GestureCallbackToSignal> callbacks;
    bool isrunning;
    Gesture::MouseGestureRecognizer mgr;

};

GestureHandler::GestureHandler()
{
    d = new Private;
    d->isrunning = false;

    inicializeGestures();


}

void GestureHandler::inicializeGestures()
{

    DirectionList dl;
    dl<<Up<<Left;

    d->callbacks.append(GestureCallbackToSignal(this,Brush));
    d->mgr.addGestureDefinition(Gesture::GestureDefinition(dl,&d->callbacks[d->callbacks.size()-1]));
}

bool GestureHandler::observe(CanvasMgr* cm)
{
    connect(cm, SIGNAL(canvasCreated(Canvas*)), this, SLOT(handleCanvas(Canvas*)));
    connect(cm, SIGNAL(canvasRemoved(Canvas*)), this, SLOT(letCanvasGo(Canvas*)));
    return true;
}

bool GestureHandler::start()
{
    d->isrunning=true;return true;
}

bool GestureHandler::stop()
{
    d->isrunning=false;return true;
}

void GestureHandler::handleCanvas(Canvas *cnvs)
{
    connect(cnvs, SIGNAL(gestureCreated(QPainterPath)), this, SLOT(handleGesture(QPainterPath)));
    d->activeCanvases.push_back(cnvs);
}

void GestureHandler::letCanvasGo(Canvas* cnvs)
{
    d->activeCanvases.remove(d->activeCanvases.indexOf(cnvs));
    disconnect(cnvs, SIGNAL(gestureCreated(QPainterPath)), this, SLOT(handleGesture(QPainterPath)));
}

void GestureHandler::handleGesture(QPainterPath gesture)
{
    if(d->isrunning)
    {
        qDebug() << "Received gesture :" <<endl;

        QPainterPath::Element elem;
        int x,y;
        for(int i = 0; i < gesture.elementCount(); i++)
        {
            elem = gesture.elementAt(i);
            x = static_cast<int>(elem.x);
            y = static_cast<int>(elem.y);
            //first step
            if(elem.isMoveTo())
            {
                //qDebug()<< "move x: " << elem.x <<"y: " <<elem.y <<" ";

                d->mgr.startGesture(x,y);
            }
            if(elem.isLineTo())
            {
                //qDebug()<< "bod x: " << elem.x <<"y: " <<elem.y <<" ";
                //last item - finish gesture
                if(i==gesture.elementCount()-1)
                {
                    d->mgr.endGesture(x,y);
                }
                else
                    d->mgr.addPoint(x,y);
            }



        }
    }
}

#include "gesturehandler.moc"
