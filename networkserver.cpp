#include "networkservice.h"
#include "networkserver.h"
#include "networkserverthread.h"

NetworkServer::NetworkServer(QObject* parent)
      : QTcpServer(parent)
{
}

void NetworkServer::incomingConnection(int sock)
{
   qDebug() << "server::incomingConnection()";
   NetworkServerThread *thread = new NetworkServerThread(this, sock);
   connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
   thread->start();
}

#include "networkserver.moc"
