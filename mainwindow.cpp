#include <QCloseEvent>
#include <QSettings>
#include "mainwindow.h"

// Const
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

MainWindow::MainWindow(QWidget* parent)
      : QMainWindow(parent)
{
   // Load window defaults
   setWindowTitle(tr("Shared paint"));
   //setWindowIcon(QIcon(":/icons/spaint.png"));
   resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);

   // Load UI
   load();

   // Load settings
   loadSettings();
}

void MainWindow::load()
{
   // Main widget
   QWidget* mainWidget = new QWidget(this);
   setCentralWidget(mainWidget);

   // Menu
   setMenuBar(createMenuBar());
}

bool MainWindow::observe(CanvasMgr* cm)
{
   return true;
}

QMenuBar* MainWindow::createMenuBar()
{
   QMenuBar* bar = new QMenuBar(this);

   QMenu* sessionMenu = bar->addMenu(tr("Session"));
   sessionMenu->addAction(tr("C&onnect"));
   sessionMenu->addAction(tr("&Save as image"));
   sessionMenu->addSeparator();
   sessionMenu->addAction(tr("&Quit"));

   QMenu* canvasMenu = bar->addMenu(tr("Canvas"));
   sessionMenu->addAction(tr("&Clear"));

   QMenu* windowMenu = bar->addMenu(tr("Window"));

   QMenu* optionsMenu = bar->addMenu(tr("Options"));
   optionsMenu->addAction(tr("Options"));
   optionsMenu->addSeparator();
   optionsMenu->addAction(tr("Help"));

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
