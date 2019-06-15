#include "NetListen.h"

NetListen::NetListen(QHostAddress ip, quint16 nPort, QObject *parent) : QObject(parent), m_ip(ip), m_nPort(nPort)
{
    m_pTcpSvr = nullptr;
}

void NetListen::slot_startListen()
{
    m_pTcpSvr = new NetTcpSvr(this);
    connect(m_pTcpSvr, SIGNAL(sign_incSockDesp(qintptr)), this, SIGNAL(sign_incSockDesp(qintptr)));
    connect(m_pTcpSvr, SIGNAL(newConnection()), this, SLOT(slot_newConnection()));
    if(!m_pTcpSvr->listen(m_ip, m_nPort))
    {
        m_pTcpSvr->deleteLater();
        m_pTcpSvr = nullptr;
    }
}

void NetListen::slot_newConnection()
{
    //qDebug("Client Connected");
}

