#include <QMessageBox>
#include <QtNetwork>

#include "canvasmgr.h"
#include "networkservice.h"

/*
struct NetworkService::Private{

   Private() : tst(0)
   {}

   int tst;
};
*/

NetworkService::NetworkService(QWidget *parent)
      : QDialog(parent), server(new NetworkServer(this))/*, d(new Private)*/
{
   /*d->tst = 4;*/
}

bool NetworkService::observe(CanvasMgr* cm)
{
   connect(cm, SIGNAL(canvasCreated(Canvas*)), this, SLOT(offerCanvas()));
   connect(cm, SIGNAL(canvasRemoved(Canvas*)), this, SLOT(dissofferCanvas()));
   return true;
}

bool NetworkService::start()
{
   if (!server->listen()) {
        QMessageBox::critical(this, tr("Shared Paint Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        server->close();
        return false;
   }

   emit(serverStarted());
   return true;
}

bool NetworkService::stop()
{
   /*server->close();*/
   emit(serverStopped());
   return true;
}

bool NetworkService::offerCanvas(Canvas* canvas)
{
   return true;
}

bool NetworkService::disofferCanvas(Canvas* canvas)
{
   return true;
}

NetworkService::~NetworkService()
{
   delete server;
}

#include "networkservice.moc"
