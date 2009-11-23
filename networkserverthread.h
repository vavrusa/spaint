#ifndef NETWORKSERVERTHREAD_H
#define NETWORKSERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>

class NetworkServerThread : public QThread
{
   Q_OBJECT

public:
   NetworkServerThread(QObject* parent, int sock);
   //~NetworkServerThread();
   void run();

signals:
   void error(QTcpSocket::SocketError sockErr);

private:
   int sock;

};

#endif // NETWORKSERVERTHREAD_H
