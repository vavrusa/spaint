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
   qDebug() << "New client";
   d->tcpSocket = new QTcpSocket(this);
   connect(d->tcpSocket, SIGNAL(readyRead()), this, SLOT(read()));
   connect(d->tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
           this, SLOT(error(QAbstractSocket::SocketError)));

   d->tcpSocket->abort();
   d->tcpSocket->connectToHost(QString("127.0.0.1"), 6666);
}

NetworkClient::~NetworkClient()
{
   delete d;
}

void NetworkClient::read()
{
   qDebug() << "read()";

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

void NetworkClient::error(QAbstractSocket::SocketError)
{
   qDebug() << "client ERROR";
}

#include "networkclient.moc"
