#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <QTcpServer>

class NetworkServer : public QTcpServer
{
   Q_OBJECT

public:
   NetworkServer(QObject* parent);

protected:
   void incomingConnection(int sock);
};

#endif // NETWORKSERVER_H
