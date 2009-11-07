#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include <QObject>
#include "canvasmgr.h"

class NetworkService : public QObject
{
   Q_OBJECT

   public:
   NetworkService();

   bool observe(CanvasMgr* cvmgr);

   public slots:
   bool start();
   bool stop();
};

#endif // NETWORKSERVICE_H
