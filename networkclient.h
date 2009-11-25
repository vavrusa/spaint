#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QObject>

class NetworkClient : public QObject
{
   Q_OBJECT

public:
   NetworkClient(QWidget *parent = 0);
   //~NetworkClient();

};

#endif // NETWORKCLIENT_H
