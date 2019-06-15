#ifndef NETTCPSVR_H
#define NETTCPSVR_H
#include <QObject>
#include <QTcpServer>

class NetTcpSvr : public QTcpServer
{
    Q_OBJECT
public:
    explicit NetTcpSvr(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr handle);

signals:
    void sign_incSockDesp(qintptr);
public slots:
};

#endif // NETTCPSVR_H
