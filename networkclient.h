#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>

class NetworkClient : public QObject
{
   Q_OBJECT

public:
   NetworkClient(QObject *parent = 0);
   ~NetworkClient();

private slots:
   void read();
   void error(QAbstractSocket::SocketError);

private:
   struct Private;
   Private* d;
};

#endif // NETWORKCLIENT_H
