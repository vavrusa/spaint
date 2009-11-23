#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include <QDialog>

#include "canvas.h"
#include "canvasmgr.h"
#include "networkserver.h"

class NetworkService : public QDialog
{
   Q_OBJECT

public:
   NetworkService(QWidget *parent = 0);
   ~NetworkService();

   bool observe(CanvasMgr* cm);

public slots:
   bool start();
   bool stop();

//private slots:
   bool offerCanvas(Canvas*);
   bool disofferCanvas(Canvas*);

signals:
   void serverStarted();
   void serverStopped();

private:
/*
   struct Private;
   Private* d;
*/
   NetworkServer* server;

};

#endif // NETWORKSERVICE_H
