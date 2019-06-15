#ifndef DEF_H
#define DEF_H

#include <QObject>

#define     FILE_CONFIG         "NiceServer.ini"
#define     LIMIT_THREAD        1000        //自定义

#define     TP_CONN_ADD         0
#define     TP_CONN_DEL         1
#define     TP_RECV_ADD         2

typedef struct tagSVR_CONF
{
   bool     bLog;               //是否启用日志
   quint16  nPort;              //端口号
   quint16  nNetThd;            //通信线程数
   QString  addrSvr;            //服务器监听地址
}SVR_CONF;

#endif // DEF_H
