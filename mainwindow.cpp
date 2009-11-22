#include <QGraphicsView>
#include <QFileDialog>
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

   // Menu
   setMenuBar(createMenuBar());
}

MainWindow::~MainWindow()
{
   delete d;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Shared Paint"),
            tr("The <b>Shared Paint</b> project "
               "is aimed to create simple editor... WTF Vojtechu?"));
}

bool MainWindow::observe(CanvasMgr* cm)
{
   connect(cm,   SIGNAL(canvasCreated(Canvas*)), d->containment, SLOT(addCanvas(Canvas*)));
   connect(cm,   SIGNAL(canvasRemoved(Canvas*)), d->containment, SLOT(removeCanvas(Canvas*)));
   return true;
}

QMenuBar* MainWindow::createMenuBar()
{
   QMenuBar* bar = new QMenuBar(this);

   QMenu* sessionMenu = bar->addMenu(tr("&File"));
   sessionMenu->addAction(QIcon(":/icons/16x16/canvas-add.png"), tr("C&onnect"))->setEnabled(false);
   sessionMenu->addAction(QIcon(":/icons/16x16/save-as.png"), tr("&Save Image"), this, SLOT(renderCanvas()), tr("Ctrl+S"));
   sessionMenu->addSeparator();
   sessionMenu->addAction(QIcon(":/icons/16x16/exit.png"), tr("&Quit"), this, SLOT(close()), tr("Ctrl+Q"));

   QMenu* editMenu = bar->addMenu(tr("&Edit"));
   editMenu->addAction(QIcon(":/icons/16x16/canvas-clear.png"), tr("&Clear"));
   editMenu->addSeparator();
   editMenu->addAction(QIcon(":/icons/16x16/configure.png"), tr("&Options"))->setEnabled(false);

   QMenu* helpMenu = bar->addMenu(tr("&Help"));
   helpMenu->addAction(QIcon(":/icons/16x16/help-contents.png"), tr("&Help"))->setEnabled(false);
   helpMenu->addSeparator();
   helpMenu->addAction(QIcon(":/icons/16x16/application.png"), tr("&About"), this, SLOT(about()));
   helpMenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));

   return bar;
}

void MainWindow::renderCanvas()
{
   // Get file location
   QString fileName = QFileDialog::getSaveFileName(this,
                      tr("Save Image As"), QDir::homePath(),
                      tr("Image Files (*.png)"));
   
   // Check extension
   if(!fileName.endsWith(".png",Qt::CaseInsensitive))
      fileName.append(".png");

   // Emit render request event
   if(!fileName.isEmpty()) {
      QFile file(fileName);
      if(file.open(QFile::ReadWrite)) {
         d->containment->renderCanvas(file);
         file.close();
      }
   }
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
