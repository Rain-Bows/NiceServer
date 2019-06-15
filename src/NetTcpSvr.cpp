#include "NetTcpSvr.h"

NetTcpSvr::NetTcpSvr(QObject *parent) : QTcpServer(parent)
{

}

void NetTcpSvr::incomingConnection(qintptr handle)
{
    emit sign_incSockDesp(handle);
}
