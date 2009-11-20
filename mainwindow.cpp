#include <QGraphicsView>
#include <QCloseEvent>
#include <QApplication>
#include <QSettings>
#include <QTimer>
#include <QMessageBox>
#include "mainwindow.h"
#include "canvascontainment.h"

// Const
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

struct MainWindow::Private {

   Private() : containment(0)
   {}

   CanvasContainment* containment;
};

MainWindow::MainWindow(QWidget* parent)
      : QMainWindow(parent), d(new Private)
{
   // Load window defaults
   setWindowTitle(tr("Shared paint"));
   //setWindowIcon(QIcon(":/icons/spaint.png"));
   resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

   // Load UI
   d->containment = new CanvasContainment(this);
   setCentralWidget(d->containment->view());

   // Load settings
   loadSettings();
   QTimer::singleShot(0, this, SLOT(init()));
}

MainWindow::~MainWindow()
{
   delete d;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Shared Paint"),
            tr("The <b>Shared Paint</b> project "
               "is aimed to create simple editor..."));
}

void MainWindow::init()
{
   emit initialized();

   // Menu
   setMenuBar(createMenuBar());
}

bool MainWindow::observe(CanvasMgr* cm)
{
   connect(this, SIGNAL(initialized()),          cm,             SLOT(init()));
   connect(cm,   SIGNAL(canvasCreated(Canvas*)), d->containment, SLOT(addCanvas(Canvas*)));
   connect(cm,   SIGNAL(canvasRemoved(Canvas*)), d->containment, SLOT(removeCanvas(Canvas*)));
   return true;
}

QMenuBar* MainWindow::createMenuBar()
{
   QMenuBar* bar = new QMenuBar(this);

   QMenu* fileMenu = bar->addMenu(tr("&File"));
   fileMenu->addAction(tr("E&xit"), this, SLOT(close()), tr("Ctrl+Q"));

   QMenu* sessionMenu = bar->addMenu(tr("&Session"));
   sessionMenu->addAction(tr("C&onnect"));
   sessionMenu->addAction(tr("&Save as image"));
   sessionMenu->addSeparator();
   sessionMenu->addAction(tr("&Quit"));

   QMenu* canvasMenu = bar->addMenu(tr("&Canvas"));
   canvasMenu->addAction(tr("&Clear"));

   QMenu* windowMenu = bar->addMenu(tr("&Window"));

   QMenu* optionsMenu = bar->addMenu(tr("&Options"));
   optionsMenu->addAction(tr("&Options"));

   QMenu* helpMenu = bar->addMenu(tr("&Help"));
   helpMenu->addAction(tr("&Help"));
   helpMenu->addSeparator();
   helpMenu->addAction(tr("&About"), this, SLOT(about()));
   helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));

   return bar;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
   // Save settings
   saveSettings();

   // TODO disconnect canvases
   event->accept();
}

void MainWindow::saveSettings()
{
   QSettings set;

   // Window settings
   set.setValue("MainWindow/State",     saveState());
   set.setValue("MainWindow/Size",      size());
   set.setValue("MainWindow/Position",  pos());
   set.setValue("MainWindow/Maximized", windowState().testFlag(Qt::WindowMaximized));
}

void MainWindow::loadSettings()
{
   QSettings set;

   // Window settings
   if(set.contains("MainWindow/State"))
   {
      resize(set.value("MainWindow/Size").toSize());
      move(set.value("MainWindow/Position").toPoint());

      if(set.value("MainWindow/Maximized").toBool())
         setWindowState(windowState() | Qt::WindowMaximized);

      restoreState(set.value("MainWindow/State").toByteArray());
   }
   else
   {
      // Defaults
      resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
   }
}

#include "mainwindow.moc"
