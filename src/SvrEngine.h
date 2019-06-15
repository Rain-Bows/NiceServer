/*************************************************
ClassName:   SvrEngine
Description: 通信引擎,线程池
Date:        2019.6
Author:      Sepuer
E-mail:      ucun@qq.com
*************************************************/
#ifndef SVRENGINE_H
#define SVRENGINE_H

#include <QObject>
#include <QThread>
#include "Def.h"
#include "NetListen.h"
#include "NetSocket.h"

class SvrEngine : public QObject
{
    Q_OBJECT
public:
    explicit SvrEngine(QObject *parent = nullptr);
    ~SvrEngine();
    void svrStart(const SVR_CONF conf);

private:
    //开始监听
    void startListen(const QString ip, const quint16 nPort);
    //创建通信线程
    void startSockThd(const int nNub);
signals:
    void sign_startListen();
    void sign_createSock(QObject *, qintptr);
    void sign_abortSock(QObject *);
    void sign_abortSock();
    void sign_sendData(QObject *, QByteArray);
    void sign_output(QObject *, QByteArray);
    void sign_count(int);
public slots:
    //负载均衡
    void slot_selectSock(qintptr sockDesp);
    //接收数据
    void slot_recvData(QObject *pSock, QByteArray baData);
    //释放资源
    void slot_dealloc(QObject *pSock);

private:
    SVR_CONF            m_tSvrConf;
    NetListen*          m_pNetListenThd;
    QThread*            m_pListenThd;
    QList<NetSocket *>  m_lpNetSockThd;
    QList<QThread *>    m_lpSockThd;

};

#endif // SVRENGINE_H
