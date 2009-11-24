#include <QtNetwork>
#include <QList>

#include "canvasmgr.h"
#include "networkservice.h"
#include "networkserver.h"
#include "networkclient.h"

class NetworkService::Private
{
   public:
      Private() : server(0)
      {}

      NetworkServer* server;
      //QList<NetworkClient*> clients;
};

NetworkService::NetworkService(QWidget* parent)
      : d(new Private)
{
   d->server = new NetworkServer(parent);
}

NetworkService::~NetworkService()
{
   delete d->server;
}

bool NetworkService::observe(CanvasMgr* cm)
{
   connect(cm, SIGNAL(canvasCreated(Canvas*)), this, SLOT(offerCanvas()));
   connect(cm, SIGNAL(canvasRemoved(Canvas*)), this, SLOT(dissofferCanvas()));
   return true;
}

bool NetworkService::startClient()
{
   return true;
}

bool NetworkService::startServer()
{
   emit(serverState(NetworkServer::start));

   if (!d->server->listen()) {
        d->server->close();
        emit(serverState(NetworkServer::errStart, d->server->errorString()));
        return false;
   }

   emit(serverState(NetworkServer::run));
   return true;
}

bool NetworkService::stopServer()
{
   d->server->close();
   emit(serverState(NetworkServer::stop));
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

#include "networkservice.moc"
