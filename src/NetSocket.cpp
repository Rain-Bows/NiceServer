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
            m_mutexSock.lock();
            m_lSock.append(pSock);
            m_mutexSock.unlock();

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
    QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
    emit sign_dealloc(pSock);
    qDebug("\"[%p]: disconnected\"", pSock);
}

void NetSocket::slot_sendData(QObject *pSock, QByteArray baData)
{
    m_mutexSock.lock();
    if(!m_lSock.contains(pSock))
    {
        m_mutexSock.unlock();
        return;
    }
    QTcpSocket *pTcpSock = qobject_cast<QTcpSocket*>(pSock);
    pTcpSock->write(baData);
    m_mutexSock.unlock();
}

void NetSocket::slot_abortSock(QObject *pSock)
{
    m_mutexSock.lock();
    if(!m_lSock.contains(pSock))
    {
        m_mutexSock.unlock();
        return;
    }
    QTcpSocket * pTcpSock = qobject_cast<QTcpSocket*>(pSock);
    pTcpSock->abort();
    m_mutexSock.unlock();
}

void NetSocket::slot_abortSock()
{
    m_mutexSock.lock();
    for (int i=0; i<m_lSock.size(); i++)
    {
        QTcpSocket * pTcpSock = qobject_cast<QTcpSocket*>(m_lSock[i]);
        pTcpSock->abort();
    }
    m_mutexSock.unlock();
}
