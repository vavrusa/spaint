#ifndef NETWORKCLIENT_H
#define NETWORKCLIENT_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
QT_END_NAMESPACE

class NetworkClient : public QDialog
{
   Q_OBJECT

public:
   NetworkClient(QWidget *parent = 0);
   //~NetworkClient();

};

#endif // NETWORKCLIENT_H
