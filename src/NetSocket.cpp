#include "NetSocket.h"

NetSocket::NetSocket(QObject *parent) : QObject(parent)
{

}

int NetSocket::sockSum()
{
    QMutexLocker locker(&m_mutexSock);
    return m_lSock.size();
}

void NetSocket::slot_createSock(QObject *obj, qintptr sockDesp)
{
    if(obj != this)
    {
        return;
    }
    QTcpSocket *pSock = new QTcpSocket(this);
    if(pSock)
    {
        if(pSock->setSocketDescriptor(sockDesp))
        {
            QMutexLocker locker(&m_mutexSock);
            m_lSock.append(pSock);

            connect(pSock, SIGNAL(readyRead()), this, SLOT(slot_readReady()));
            connect(pSock, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
            qDebug("\"[%p]: created\"", pSock);
        }
    }
}

void NetSocket::slot_readReady()
{
    QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
    if(pSock)
    {
        QByteArray baRecv = pSock->readAll();
        emit sign_readReady(pSock, baRecv);
    }
}

void NetSocket::slot_disconnected()
{
    QMutexLocker locker(&m_mutexSock);
    QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
    m_lSock.removeOne(pSock);
    pSock->deleteLater();

    emit sign_dealloc(pSock);
    qDebug("\"[%p]: disconnected\"", pSock);
}

void NetSocket::slot_sendData(QObject *pSock, QByteArray baData)
{
    QMutexLocker locker(&m_mutexSock);
    if(!m_lSock.contains(pSock))
    {
        return;
    }
    QTcpSocket *pTcpSock = qobject_cast<QTcpSocket*>(pSock);
    pTcpSock->write(baData);
}

void NetSocket::slot_abortSock(QObject *pSock)
{
    QMutexLocker locker(&m_mutexSock);
    if(!m_lSock.contains(pSock))
    {
        return;
    }
    QTcpSocket * pTcpSock = qobject_cast<QTcpSocket*>(pSock);
    pTcpSock->abort();
}

void NetSocket::slot_abortSock()
{
    QMutexLocker locker(&m_mutexSock);
    for (int i=0; i<m_lSock.size(); i++)
    {
        QTcpSocket * pTcpSock = qobject_cast<QTcpSocket*>(m_lSock[i]);
        pTcpSock->abort();
    }
}
