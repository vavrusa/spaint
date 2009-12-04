#include "gestureeditor.h"
#include "mousegesturerecognizer.h"
#include "gesturehandler.h"
#include "gestureeditorcanvas.h"
#include <QMouseEvent>
#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QHBoxLayout>
#include <QtGui>
#include <QMap>

using namespace Gesture;

class GestureEditor::Private
{
   public:
    QFrame*         frame;
    QComboBox*      combo;
    QPushButton*    ok;
    QPushButton*    clear;

    QHBoxLayout*    main;
    QVBoxLayout*    right;

    Handler*        handler;
    gestureType     activeType;
    QStringList     gestureTypeList;

    unsigned        state;
    DirectionList   activeDirections;
    QPoint          mousePos;
    QPoint          mouseStart;
    QPoint          pathFinish;

    static const unsigned step = 50;
    static const unsigned diff = 3;


};

bool GestureEditor::isBlock(QPoint a, QPoint b)
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

void GestureEditor::show()
{
    currentGestureChanged();
    QWidget::show();
}

void GestureEditor::defineLayout()
{
    this->setFixedSize(400,300);

    d->frame = new QFrame;
    d->combo = new QComboBox;
    d->ok = new QPushButton("Save");
    d->clear = new QPushButton("Clear");

    d->main = new QHBoxLayout;
    d->right = new QVBoxLayout;

    d->main->addWidget(d->frame);
    d->main->addLayout(d->right);

    d->right->addWidget(d->combo);
    d->right->addWidget(d->ok);
    d->right->addWidget(d->clear);
    d->right->addStretch();


    d->frame->setFixedSize(300,300);

    d->combo->addItem("Pen",QVariant(Pen));
    d->combo->addItem("Brush",QVariant(Brush));

    this->setLayout(d->main);

}

void GestureEditor::makeConnections()
{
    connect(d->combo,SIGNAL(currentIndexChanged(int)),this, SLOT(currentGestureChanged()));
    connect(d->clear,SIGNAL(clicked()),this,SLOT(eraseCurrentGestures()));
    connect(d->ok,SIGNAL(clicked()),this,SLOT(editCurrentGesture()));
}

void GestureEditor::paintEvent(QPaintEvent* )
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

    curX = 150 - (maxX + minX) * (d->step/2);
    curY = 150 - (maxY + minY) * (d->step/2);


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

void GestureEditor::mousePressEvent(QMouseEvent *event)
{
    //check for left mouse button
    if(event->button() == Qt::LeftButton)
    {
        //check, if we started drawing from around finish of last segment
        //or if we just started drawing new gesture
        if((abs(event->pos().x()-d->pathFinish.x()) < 10 && abs(event->pos().y()-d->pathFinish.y())<10)||d->activeDirections.empty())
        {
            d->mouseStart = event->pos();
            d->mousePos = event->pos();
            d->state = Drawing;
            update();
        }
    }

    QWidget::mousePressEvent(event);
}

void GestureEditor::mouseMoveEvent(QMouseEvent *event)
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

void GestureEditor::mouseReleaseEvent(QMouseEvent *event)
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

void GestureEditor::eraseCurrentGestures()
{
    d->activeDirections.clear();
    update();
}

void GestureEditor::currentGestureChanged()
{

    d->activeDirections = d->handler->getGesture(static_cast<gestureType>(d->combo->itemData(d->combo->currentIndex()).toUInt()));
    d->activeType =static_cast<gestureType>(d->combo->itemData(d->combo->currentIndex()).toUInt());
    update();
}

void GestureEditor::editCurrentGesture()
{
    d->handler->setGesture(d->activeType,d->activeDirections);
}

GestureEditor::GestureEditor(Handler*parent)
    : d(new Private)
{
    d->handler = parent;

    //setting up widget layout
    defineLayout();

    //inicialization
    d->activeType = Pen;
    d->state = Idle;

    makeConnections();
}

GestureEditor::~GestureEditor()
{
  delete d;
}

#include "gestureeditor.moc"
