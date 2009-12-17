#include <QMouseEvent>
#include <QGraphicsPathItem>
#include <QDialogButtonBox>
#include <QPainterPath>
#include <QBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include <QIcon>
#include <QMap>

#include "gestureeditor.h"
#include "mousegesturerecognizer.h"
#include "gesturehandler.h"
#include "gestureeditorcanvas.h"

using namespace Gesture;

class GestureEditor::Private
{
   public:
    GestureEditorCanvas* canvas;
    Handler*        handler;
    Type     activeType;
    QStringList     gestureTypeList;
    QListWidget*    listWidget;

};

GestureEditor::GestureEditor(Handler* h, QWidget* parent)
    : QDialog(parent), d(new Private)
{
    d->handler = h;

    //setting up widget layout
    setWindowIcon(QIcon(":/icons/16x16/configure.png"));
    setWindowTitle(tr("Gesture Editor"));
    defineLayout();

    //inicialization
    d->activeType = Pen;
}

GestureEditor::~GestureEditor()
{
  delete d;
}

void GestureEditor::show()
{
    currentGestureChanged();
    QDialog::show();
}

void GestureEditor::defineLayout()
{

   // Create canvas
   d->canvas = new GestureEditorCanvas(this);
   d->canvas->setFixedSize(270,270);

   // Create list and combo
   d->listWidget = new QListWidget(this);
   d->listWidget->setIconSize(QSize(32,32));
   const QMap<Type,Info>& map = d->handler->getTypes();
   QMap<Type,Info>::const_iterator it = map.constBegin();
   while(it != map.constEnd())
   {
      d->listWidget->addItem(new QListWidgetItem(it.value().second,
                                                 it.value().first,
                                                 d->listWidget,
                                                 it.key()));
      ++it;
   }

   d->listWidget->setCurrentRow(0);

   // Button box
   QDialogButtonBox* box = new QDialogButtonBox(this);
   QPushButton* clear = box->addButton(tr("&Clear"), QDialogButtonBox::DestructiveRole);
   QPushButton* reset = box->addButton(QDialogButtonBox::Reset);
   QPushButton* save = box->addButton(QDialogButtonBox::Apply);
   box->addButton(QDialogButtonBox::Ok);
   box->addButton(QDialogButtonBox::Cancel);


   // Create canvas layout
   QHBoxLayout* layout = new QHBoxLayout();
   layout->addWidget(d->listWidget);
   layout->addWidget(d->canvas);

   // Create buttons layout
   QHBoxLayout* ctLayout = new QHBoxLayout();
   ctLayout->addWidget(box);

   // Create main layout
   QVBoxLayout* main = new QVBoxLayout(this);
   main->addLayout(layout);
   main->addLayout(ctLayout);

   // Connect widgets
   connect(d->handler,SIGNAL(somethingChanged()), this, SLOT(update()));
   connect(d->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(currentGestureChanged()));
   connect(save,SIGNAL(clicked()),this,SLOT(editCurrentGesture()));
   connect(clear,SIGNAL(clicked()),this,SLOT(eraseCurrentGestures()));
   connect(reset,SIGNAL(clicked()),this,SLOT(resetCurrentGesture()));
   connect(box, SIGNAL(accepted()), this, SLOT(editCurrentGesture()));
   connect(box, SIGNAL(accepted()), this, SLOT(hide()));
   connect(box, SIGNAL(rejected()), this, SLOT(hide()));

}

void GestureEditor::eraseCurrentGestures()
{
    DirectionList dl;
    d->canvas->setDirections(dl);
}

void GestureEditor::currentGestureChanged()
{
    d->canvas->setDirections(d->handler->getGesture(static_cast<Type>(d->listWidget->currentItem()->type())));
    d->activeType =static_cast<Type>(d->listWidget->currentItem()->type());
    editCurrentGesture();
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
#include "gestureeditor.moc"
