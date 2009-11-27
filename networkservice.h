#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include <QDialog>

#include "canvas.h"
#include "canvasmgr.h"
#include "networkserver.h"

class NetworkService : public QObject
{
   Q_OBJECT

public:
   NetworkService(QObject *parent = 0);
   ~NetworkService();

   bool observe(CanvasMgr* cm);

public slots:
   bool startServer();
   bool stopServer();
   bool startClient(QString& host, QString& port);
   bool stopClients();

private slots:
   bool offerCanvas(Canvas*);
   bool disofferCanvas(Canvas*);

signals:
   void serverState(NetworkServer::state, const QString& msg = QString());

private:
   struct Private;
   Private* d;
};

#endif // NETWORKSERVICE_H
