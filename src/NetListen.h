#ifndef NETLISTEN_H
#define NETLISTEN_H

#include <QObject>
#include <QHostAddress>
#include "NetTcpSvr.h"

class NetListen : public QObject
{
    Q_OBJECT
public:
    explicit NetListen(QHostAddress ip, quint16 nPort, QObject *parent = nullptr);

signals:
    void sign_incSockDesp(qintptr);
public slots:
    void slot_startListen();
    void slot_newConnection();
private:
    QHostAddress    m_ip;
    quint16         m_nPort;
    NetTcpSvr*      m_pTcpSvr;
};

#endif // NETLISTEN_H
