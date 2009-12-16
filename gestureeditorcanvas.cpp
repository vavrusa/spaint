#include "gestureeditorcanvas.h"
#include <QPainter>

using namespace Gesture;

class GestureEditorCanvas::Private
{
public:
    unsigned        state;
    DirectionList   activeDirections;

    QPoint          mousePos;           //current position of mouse cursor
    QPoint          mouseStart;         //begining of drawing
    QPoint          pathFinish;         //finish of path

    static const unsigned step = 50;    //size of step in drawing gesture
    static const unsigned diff = 3;     //max difference while drawing
};

GestureEditorCanvas::GestureEditorCanvas(GestureEditor *parent)
    : QWidget(parent), d(new Private)
{
    d->state = Idle;
    //drawing white background
    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(Qt::white));
    setPalette(pal);
    setAutoFillBackground(true);
}

GestureEditorCanvas::~GestureEditorCanvas()
{
    delete d;
}

bool GestureEditorCanvas::isBlock(QPoint a, QPoint b)
{
    int xx,yy;
    xx = b.x() - a.x();
    yy = b.y() - a.y();
    //qDebug() << "x:" << xx << "y:" << yy;
    //check direction - cant be the same as last element
    if(!d->activeDirections.empty())
        switch(d->activeDirections.last())
        {
        case Up:
            if( abs(yy) >= d->step && yy < 0)
                return false;
            break;
        case Down:
            if( abs(yy) >= d->step && yy > 0 )
                return false;
            break;
        case Right:
            if( abs(xx) >= d->step && xx > 0)
                return false;
            break;
        case Left:
            if( abs(xx) >= d->step && xx < 0)
                return false;
            break;
        }
    //check length
    xx = abs(xx);
    yy = abs(yy);
    return ((( xx >= d->step && yy <= d->diff ) || ( yy >= d->step && xx <= d->diff ))?true:false);
}

void GestureEditorCanvas::paintEvent(QPaintEvent* )
{
    QPainter painter(this);
    QPen pen;

    if(d->state == Drawing)
    {
        //check, if line seems as finished block
        if(isBlock(d->mouseStart,d->mousePos))
        {
            pen.setColor(QColor(215, 0, 25, 64)); // Transparent red
            pen.setWidth(8);
        }
        else
        {
            pen.setColor(QColor(25, 0, 215, 64));   //transparent blue
            pen.setWidth(6);
        }

        painter.setPen(pen);
        painter.drawPoint(d->mouseStart);
        painter.drawLine(d->mouseStart,d->mousePos);
    }

    pen.setColor(QColor(215, 0, 25, 64)); // Transparent red
    pen.setWidth(8);
    painter.setPen(pen);

    //qDebug()<<"count:"<<d->activeDirections.count();
    int curX = 0, curY = 0;

    //setting the picture into the center
    int maxX = 0, minX = 0, maxY = 0, minY = 0;
    for(int i = 0; i< d->activeDirections.count(); i++)
    {
        switch(d->activeDirections[i])
        {
        case Up:
            curY--;
            break;
        case Down:
            curY++;
            break;
        case Left:
            curX--;
            break;
        case Right:
            curX++;
            break;
        }
        if(curY < minY)
            minY--;
        if(curY > maxY)
            maxY++;
        if(curX < minX)
            minX--;
        if(curX > maxX)
            maxX++;
    }

    curX = width()/2 - (maxX + minX) * (d->step/2);
    curY = height()/2 - (maxY + minY) * (d->step/2);


    //drawing arrows
    for(int i = 0; i< d->activeDirections.count(); i++)
    {
        switch(d->activeDirections[i])
        {
        case Up:
            painter.drawLine(curX,curY,curX,curY-d->step);
            curY-=d->step;
            painter.drawLine(curX,curY,curX+10,curY+10);
            painter.drawLine(curX,curY,curX-10,curY+10);
            break;
        case Down:
            painter.drawLine(curX,curY,curX,curY+d->step);
            curY+=d->step;
            painter.drawLine(curX,curY,curX+10,curY-10);
            painter.drawLine(curX,curY,curX-10,curY-10);
            break;
        case Left:
            painter.drawLine(curX-d->step,curY,curX,curY);
            curX-=d->step;
            painter.drawLine(curX,curY,curX+10,curY+10);
            painter.drawLine(curX,curY,curX+10,curY-10);
            break;
        case Right:
            painter.drawLine(curX+d->step,curY,curX,curY);
            curX+=d->step;
            painter.drawLine(curX,curY,curX-10,curY+10);
            painter.drawLine(curX,curY,curX-10,curY-10);
            break;
        }
    }
    d->pathFinish.setX(curX);
    d->pathFinish.setY(curY);
}

void GestureEditorCanvas::mousePressEvent(QMouseEvent *event)
{
    //check for left mouse button
    if(event->button() == Qt::LeftButton)
    {
        //set starting position end of last drawing

        d->mouseStart = d->pathFinish;
        d->mousePos = event->pos();
        d->state = Drawing;
        update();
    }

    QWidget::mousePressEvent(event);
}

void GestureEditorCanvas::mouseMoveEvent(QMouseEvent *event)
{
    //check state and set current mouse position
    if(d->state == Drawing)
    {
        d->mousePos = event->pos();
        update();
    }

    else
    {
        //change appereance of cursor
        if(abs(event->pos().x() - d->pathFinish.x()) < 10 && abs(event->pos().y() - d->pathFinish.y()) < 10)
            setCursor(Qt::PointingHandCursor);
        else
            setCursor(Qt::ArrowCursor);
        update();
    }

    QWidget::mouseMoveEvent(event);
}

void GestureEditorCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    //check state and check, if suitable for saving
    if(d->state == Drawing )
    {
        d->state = Idle;

        if(isBlock(d->mouseStart, d->mousePos))
        {
            int xx,yy;
            xx = d->mousePos.x() - d->mouseStart.x();
            yy = d->mousePos.y() - d->mouseStart.y();
            if( abs(xx) > d->step && xx > 0)
                d->activeDirections<<Right;
            else if( abs(xx) > d->step)
                d->activeDirections<<Left;
            else if( abs(yy) > d->step && yy > 0)
                d->activeDirections<<Down;
            else
                d->activeDirections<<Up;
        }
        update();
    }

    QWidget::mouseReleaseEvent(event);
}


DirectionList GestureEditorCanvas::getDirections()
{
    return d->activeDirections;
}

void GestureEditorCanvas::setDirections(DirectionList dl)
{
    d->activeDirections = dl;
    update();
}

#include "gestureeditorcanvas.moc"
