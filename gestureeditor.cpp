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
    GestureEditorCanvas* canvas;
    QListWidget*     list;
    QComboBox*      combo;
    QPushButton*    ok;
    QPushButton*    clear;
    QPushButton*    reset;

    QHBoxLayout*    main;
    QVBoxLayout*    right;
    QVBoxLayout*    left;

    Handler*        handler;
    gestureType     activeType;
    QStringList     gestureTypeList;

};



void GestureEditor::show()
{
    currentGestureChanged();
    QWidget::show();
}

void GestureEditor::defineLayout()
{
    this->setFixedSize(400,300);

    d->canvas = new GestureEditorCanvas(this);
    d->list = new QListWidget;
    d->combo = new QComboBox;
    d->ok = new QPushButton("Save");
    d->clear = new QPushButton("Clear");
    d->reset = new QPushButton("Default");

    d->main = new QHBoxLayout;
    d->right = new QVBoxLayout;
    d->left = new QVBoxLayout;

    d->main->addLayout(d->left);
    d->main->addWidget(d->canvas);

    d->left->addWidget(d->list);
    d->left->addWidget(d->ok);
    d->left->addWidget(d->clear);
    d->left->addWidget(d->reset);

    d->canvas->setFixedSize(270,270);

    d->combo->addItem("Pen",QVariant(Pen));
    d->combo->addItem("Brush",QVariant(Brush));

    d->list->setFixedSize(70,180);
    d->list->setSelectionMode(QAbstractItemView::SingleSelection);
    QListWidgetItem * pen = new QListWidgetItem(QIcon(":/icons/16x16/canvas-add.png"),tr("Pen"),d->list, Pen);
    QListWidgetItem * brush= new QListWidgetItem(QIcon(":/icons/16x16/canvas-add.png"),tr("Brush"),d->list, Brush);

    d->list->setCurrentRow(0);

    //d->list->addItem(&pen);
    //d->list->addItem(&brush);

    this->setLayout(d->main);
    //! EDIT
    setWindowIcon(QIcon(":/icons/16x16/canvas-add.png"));
    setWindowTitle(tr("Gesture Editor"));

}

void GestureEditor::makeConnections()
{
    connect(d->handler,SIGNAL(somethingChanged()), this, SLOT(update()));
    connect(d->list, SIGNAL(currentRowChanged(int)), this, SLOT(currentGestureChanged()));
    connect(d->clear,SIGNAL(clicked()),this,SLOT(eraseCurrentGestures()));
    connect(d->ok,SIGNAL(clicked()),this,SLOT(editCurrentGesture()));
    connect(d->reset,SIGNAL(clicked()),this,SLOT(resetCurrentGesture()));
}




void GestureEditor::eraseCurrentGestures()
{
    DirectionList dl;
    d->canvas->setDirections(dl);
}

void GestureEditor::currentGestureChanged()
{
    d->canvas->setDirections(d->handler->getGesture(static_cast<gestureType>(d->list->currentItem()->type())));
    d->activeType =static_cast<gestureType>(d->list->currentItem()->type());
}

void GestureEditor::editCurrentGesture()
{
    d->handler->setGesture(d->activeType,d->canvas->getDirections());
}

void GestureEditor::resetCurrentGesture()
{
    d->handler->resetGesture(d->activeType);
    currentGestureChanged();
}

GestureEditor::GestureEditor(Handler*parent)
    : d(new Private)
{
    d->handler = parent;

    //setting up widget layout
    defineLayout();

    //inicialization
    d->activeType = Pen;


    makeConnections();
}

GestureEditor::~GestureEditor()
{
  delete d;
}

#include "gestureeditor.moc"
