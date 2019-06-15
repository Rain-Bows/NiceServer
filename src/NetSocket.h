/*************************************************
ClassName:   NetSocket
Description: 管理网络套接字
Date:        2019.6
Author:      Sepuer
E-mail:      ucun@qq.com
*************************************************/
#ifndef NETSOCKET_H
#define NETSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>

class NetSocket : public QObject
{
    Q_OBJECT
public:
    explicit NetSocket(QObject *parent = nullptr);
    int sockSum();
signals:
    void sign_readReady(QObject *, QByteArray);
    void sign_dealloc(QObject *);       //释放资源
public slots:
    void slot_createSock(QObject *, qintptr);
    void slot_readReady();
    void slot_disconnected();
    void slot_sendData(QObject *, QByteArray);
    void slot_abortSock(QObject *);     //断开目标连接
    void slot_abortSock();              //断开所有连接
private:
    QList<QObject *>    m_lSock;
    QMutex              m_mutexSock;
};

#endif // NETSOCKET_H
