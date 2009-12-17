#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>

#include "canvasmgr.h"
#include "canvas.h"

class NetworkClient : public QObject
{
   Q_OBJECT

public:
   enum state {
      START,
      RUN,
      STOP,
      ERR_START,
      ERR_RUN,
      ERR_STOP
   };

   NetworkClient(QObject *parent = 0);
   ~NetworkClient();

   bool observe(CanvasMgr* cm);
   bool start(QString& addr, quint16 port);
   bool stop();

signals:
   void createCanvas(QString name);

private slots:
   void receiveData();
   void error(QAbstractSocket::SocketError);
   void tst(QAbstractSocket::SocketState);

private:
   struct Private;
   Private* d;
};

#endif // NETWORKCLIENT_H
