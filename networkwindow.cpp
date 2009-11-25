#include <QGraphicsView>
#include <QFileDialog>
#include <QCloseEvent>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QSettings>
#include <QTimer>
#include <QtNetwork>
#include "networkwindow.h"
#include "networkservice.h"
#include "networkserver.h"

// Const
#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 300

struct NetworkWindow::Private {

   Private()
   {}

};

NetworkWindow::NetworkWindow(QWidget* parent)
      : QMainWindow(parent), d(new Private)
{
   // Load window defaults
   setWindowTitle(tr("Shared paint - network"));
   //setWindowIcon(QIcon(":/icons/spaint.png"));
   resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);


   QWidget* newConnection = new QWidget(this);

   QLabel* ipAddressLabel = new QLabel(QString(tr("IP address:")));
   QLineEdit* ipAddressEdit = new QLineEdit("");
   QLabel* portLabel = new QLabel(QString(tr("Port:")));
   QLineEdit* portEdit = new QLineEdit("");
   QPushButton* ok = new QPushButton("Ok");
   QPushButton* cancel = new QPushButton("Cancel");

   QGridLayout *layout = new QGridLayout;
   layout->addWidget(ipAddressLabel, 0, 0);
   layout->addWidget(ipAddressEdit, 0, 1);
   layout->addWidget(portLabel, 1, 0);
   layout->addWidget(portEdit, 1, 1);
   layout->addWidget(ok, 2, 0);
   layout->addWidget(cancel, 2, 1);

   ipAddressLabel->setFocus();

   newConnection->setLayout(layout);


   setCentralWidget(newConnection);

   // Load settings
   loadSettings();
}

NetworkWindow::~NetworkWindow()
{
   delete d;
}

bool NetworkWindow::observe(NetworkService* net)
{
   connect(net, SIGNAL(serverState(NetworkServer::state, QString)),
           this, SLOT(showServerState(NetworkServer::state, QString)));
   return true;
}

void NetworkWindow::showServerState(NetworkServer::state state, const QString &msg)
{
   switch (state) {
      case NetworkServer::errStart:
         QMessageBox::critical(this, tr("Shared Paint Server"),
                               tr("Unable to start the server: %1.")
                               .arg(msg));
         break;

      case NetworkServer::start:
         // starting
         break;

      case NetworkServer::run:
         // running
         break;

      case NetworkServer::stop:
         // stopped
         break;

      default:
         QMessageBox::information(this, tr("Shared Paint Server"),
                                  tr("NOT IMPLEMENTED: Unknown state"));
   }
}

void NetworkWindow::promptClientConnection()
{

}

void NetworkWindow::closeEvent(QCloseEvent *event)
{
   // Save settings
   saveSettings();

   // TODO
   event->accept();
}

void NetworkWindow::saveSettings()
{
   QSettings set;

   // Window settings
   set.setValue("NetworkWindow/State",     saveState());
   set.setValue("NetworkWindow/Size",      size());
   set.setValue("NetworkWindow/Position",  pos());
   set.setValue("NetworkWindow/Maximized", windowState().testFlag(Qt::WindowMaximized));
}

void NetworkWindow::loadSettings()
{
   QSettings set;

   // Window settings
   if(set.contains("NetworkWindow/State"))
   {
      resize(set.value("NetworkWindow/Size").toSize());
      move(set.value("NetworkWindow/Position").toPoint());

      if(set.value("NetworkWindow/Maximized").toBool())
         setWindowState(windowState() | Qt::WindowMaximized);

      restoreState(set.value("NetworkWindow/State").toByteArray());
   }
   else
   {
      // Defaults
      resize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
   }
}

#include "networkwindow.moc"
