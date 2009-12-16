#include <QMessageBox>
#include <QtNetwork>
#include <QDebug>

#include "networkservice.h"
#include "networkclient.h"

struct NetworkClient::Private
{
public:
   Private() : tcpSocket(0), blockSize(0)
   {}

   QTcpSocket* tcpSocket;
   quint16 blockSize;
};

NetworkClient::NetworkClient(QObject *parent)
      : QObject(parent), d(new Private)
{
   d->tcpSocket = new QTcpSocket(this);
   connect(d->tcpSocket, SIGNAL(readyRead()), this, SLOT(receiveData()));
   connect(d->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(error(QAbstractSocket::SocketError)));
}

NetworkClient::~NetworkClient()
{
   delete d->tcpSocket;
   delete d;
}

bool NetworkClient::start(QString& addr, quint16 port)
{
   qDebug() << "Client::start(" << addr << "," << port << ")";
   d->tcpSocket->abort();
   d->tcpSocket->connectToHost(addr, port);

   //emit client connecting

   return true;
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
   in.setVersion(QDataStream::Qt_4_0);

   if (d->blockSize == 0) {
       if (d->tcpSocket->bytesAvailable() < (int)sizeof(quint16))
           return;

       in >> d->blockSize;
   }

   if (d->tcpSocket->bytesAvailable() < d->blockSize)
       return;

   QString data;
   in >> data;

   qDebug() << data;

}

void NetworkClient::error(QAbstractSocket::SocketError err)
{
   qDebug() << "Client::error()" << err;
}

#include "networkclient.moc"
