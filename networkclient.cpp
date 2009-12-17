#include <QtNetwork>

#include "networkservice.h"
#include "networkclient.h"
#include "canvas.h"

struct NetworkClient::Private
{
public:
   Private() : tcpSocket(0)
   {}

   QTcpSocket* tcpSocket;
};

NetworkClient::NetworkClient(QObject *parent)
      : QObject(parent), d(new Private)
{
}

NetworkClient::~NetworkClient()
{
   delete d;
}

bool NetworkClient::observe(CanvasMgr* cm)
{
   connect(this, SIGNAL(createCanvas(QString,bool)),
           cm, SLOT(create(QString,bool)));
   connect(this, SIGNAL(createCanvasPath(QString,QPainterPath)),
           cm, SLOT(importPath(QString,QPainterPath)));

   return true;
}

bool NetworkClient::start(QString& addr, quint16 port)
{
   d->tcpSocket = new QTcpSocket(this);
   connect(d->tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
   connect(d->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(error(QAbstractSocket::SocketError)));
   connect(d->tcpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
           this, SLOT(tst(QAbstractSocket::SocketState)));

   qDebug() << "NetworkClient::start(" << addr << "," << port << ")";
   //d->tcpSocket->abort();
   d->tcpSocket->connectToHost(addr, port);

   //emit client connecting

   return true;
}

void NetworkClient::tst(QAbstractSocket::SocketState state)
{
   qDebug() << "Client QTcpSocket changed state." << state;
}

bool NetworkClient::stop()
{

   d->tcpSocket->abort();

   //emit client stopping

   return true;
}

void NetworkClient::receiveData()
{
   qDebug() << "Client::receiveData(" << d->tcpSocket->bytesAvailable() << ")";

   QDataStream in(d->tcpSocket);
   in.setVersion(QDataStream::Qt_4_6);

   quint32 blockSize;
   in >> blockSize;

   quint32 dataType;
   in >> dataType;

   switch(dataType) {
   case NetworkService::STRING:
      qDebug() << "NetworkClient::receiveData() String";
      break;
   case NetworkService::CANVAS:
   {
      qDebug() << "NetworkClient::receiveData() Canvas";

      // Canvas name
      QString name;
      QTextStream stream(&name);
      in >> name;
      stream << " (" << d->tcpSocket->peerName();
      stream << ", " << d->tcpSocket->peerAddress().toString();
      stream << ":"  << d->tcpSocket->peerPort() << ")";
      qDebug() << name;

      emit createCanvas(name, true);

      // Canvas Paths
      while (d->tcpSocket->bytesAvailable() > 0) {
         qDebug() << "Received another canvas path..";
         QPainterPath path;
         in >> path;
         emit createCanvasPath(name, path);
      }

      break;
   }
   case NetworkService::CANVASPATH:
   {
      qDebug() << "NetworkClient::receiveData() Canvas Path";

      // Canvas name
      QString name;
      QTextStream stream(&name);
      in >> name;
      stream << " (" << d->tcpSocket->peerName();
      stream << ", " << d->tcpSocket->peerAddress().toString();
      stream << ":"  << d->tcpSocket->peerPort() << ")";
      qDebug() << name;

      // Canvas Path
      while (d->tcpSocket->bytesAvailable() > 0) {
         QPainterPath path;
         in >> path;
         emit createCanvasPath(name, path);
      }
      break;
   }
   default:
      qDebug() << "NetworkClient::receiveData() Unknown dataType";
   }

   qDebug() << "Client::endReceiveData(" << d->tcpSocket->bytesAvailable() << ")";


}

void NetworkClient::error(QAbstractSocket::SocketError err)
{
   qDebug() << "Client::error()" << err;
}

#include "networkclient.moc"
