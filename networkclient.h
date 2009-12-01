#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>

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

public slots:
   bool start(QString& addr, quint16 port);
   bool stop();

private slots:
   void receiveData();
   void error(QAbstractSocket::SocketError);

private:
   struct Private;
   Private* d;
};

#endif // NETWORKCLIENT_H
