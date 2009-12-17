#include <QMessageBox>
#include <QtNetwork>
#include <QDebug>

#include "networkservice.h"
#include "networkclient.h"

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
   connect(this, SIGNAL(createCanvas(QString)),
           cm, SLOT(create(QString)));

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
   qDebug() << "Client::receiveData()";

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

      emit createCanvas(name);
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
      QPainterPath path;
      /*Canvas* canvas = static_cast<Canvas*>(d->data1);

      QByteArray
      out << static_cast<quint32>(canvas->name().length());
      out << canvas->name();
      QPainterPath* path = static_cast<QPainterPath*>(d->data2);
      out << static_cast<quint32>(path->length());
      out << path;
*/
      emit createCanvasPath(name, path);
      break;
   }
   default:
      qDebug() << "NetworkClient::receiveData() Unknown dataType";
   }

   QString data;
   in >> data;
}

void NetworkClient::error(QAbstractSocket::SocketError err)
{
   qDebug() << "Client::error()" << err;
}

#include "networkclient.moc"
