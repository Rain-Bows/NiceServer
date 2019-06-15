#include "SvrEngine.h"

SvrEngine::SvrEngine(QObject *parent) : QObject(parent)
{
    if (!QMetaType::isRegistered(QMetaType::type("qintptr")))
        qRegisterMetaType<qintptr>("qintptr");

    m_pNetListenThd = nullptr;
    m_pListenThd = nullptr;
}

SvrEngine::~SvrEngine()
{
    if(m_pNetListenThd != nullptr)
    {
        m_pNetListenThd->deleteLater();
        m_pNetListenThd = nullptr;
    }
    if(m_pListenThd != nullptr)
    {
        m_pListenThd->quit();
        m_pListenThd->wait();
        m_pListenThd->deleteLater();
        m_pListenThd = nullptr;
    }

    emit sign_abortSock();

    for (int i=0; i<m_lpNetSockThd.size(); i++)
    {
        if(m_lpNetSockThd[i] != nullptr)
        {
            m_lpNetSockThd[i]->deleteLater();
            m_lpNetSockThd[i] = nullptr;
        }
    }
    m_lpNetSockThd.clear();
    for (int i=0; i<m_lpSockThd.size(); i++)
    {
        m_lpSockThd[i]->quit();
        m_lpSockThd[i]->wait();
        m_lpSockThd[i]->deleteLater();
    }
    m_lpSockThd.clear();
}

void SvrEngine::svrStart(const SVR_CONF conf)
{
    startListen(conf.addrSvr, conf.nPort);
    startSockThd(conf.nNetThd);
}

void SvrEngine::startListen(const QString ip, const quint16 nPort)
{
    QHostAddress addr(ip);
    m_pNetListenThd = new NetListen(addr, nPort);
    m_pListenThd = new QThread(this);

    connect(this, SIGNAL(sign_startListen()), m_pNetListenThd, SLOT(slot_startListen()));
    connect(m_pNetListenThd, SIGNAL(sign_incSockDesp(qintptr)), this, SLOT(slot_selectSock(qintptr)));

    m_pListenThd->start();
    m_pNetListenThd->moveToThread(m_pListenThd);
    emit sign_startListen();
}

void SvrEngine::startSockThd(const int nNub)
{
    if(nNub <= 0 || nNub > LIMIT_THREAD)
    {
        qDebug() << "Socket thread too much";
        return;
    }
    for (int n=0; n<nNub; n++)
    {
        NetSocket *pSock = new NetSocket();
        QThread *pThd = new QThread(this);
        m_lpNetSockThd.append(pSock);
        m_lpSockThd.append(pThd);

        connect(pSock, SIGNAL(sign_readReady(QObject *, QByteArray)), this, SLOT(slot_recvData(QObject *, QByteArray)));
        connect(pSock, SIGNAL(sign_dealloc(QObject *)), this, SLOT(slot_dealloc(QObject *)));
        connect(this, SIGNAL(sign_createSock(QObject *, qintptr)), pSock, SLOT(slot_createSock(QObject *, qintptr)));
        connect(this, SIGNAL(sign_abortSock(QObject *)), pSock, SLOT(slot_abortSock(QObject *)));
        connect(this, SIGNAL(sign_abortSock()), pSock, SLOT(slot_abortSock()));
        connect(this, SIGNAL(sign_sendData(QObject *, QByteArray)), pSock, SLOT(slot_sendData(QObject *, QByteArray)));

        pThd->start();
        pSock->moveToThread(pThd);
    }
}

void SvrEngine::slot_selectSock(qintptr sockDesp)
{
    int nSid = -1;
    int nMinSum = 0;
    int nSockSum = 0;
    nSockSum = m_lpNetSockThd.size();
    nMinSum = INT_MAX;
    for (int i=0; i<nSockSum; i++)
    {
        int nSum = m_lpNetSockThd[i]->sockSum();
        if(nSum == 0)
        {
            nSid = i;
            break;
        }
        if(nSum < nMinSum)
        {
            nMinSum = nSum;
            nSid = i;
        }
    }
    if(nSid >= 0 && nSid<nSockSum)
    {
        emit sign_createSock(m_lpNetSockThd[nSid], sockDesp);
        emit sign_count(TP_CONN_ADD);
    }
}

void SvrEngine::slot_recvData(QObject *pSock, QByteArray baData)
{
    //qDebug() << "recv: " << baData;
    emit sign_output(pSock, baData);
}

void SvrEngine::slot_dealloc(QObject *pSock)
{
    emit sign_count(TP_CONN_DEL);
}
