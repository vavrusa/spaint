#include <QMessageBox>
#include <QtNetwork>

#include "networkservice.h"
#include "networkserverthread.h"

NetworkServerThread::NetworkServerThread(QObject* parent, int sock)
      : QThread(parent), sock(sock)
{
}

void NetworkServerThread::run()
{
    QTcpSocket tcpSocket;

    if (!tcpSocket.setSocketDescriptor(sock)) {
        emit error(tcpSocket.error());
        return;
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << QString("Some text..");
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    tcpSocket.write(block);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();
}

#include "networkserverthread.moc"
