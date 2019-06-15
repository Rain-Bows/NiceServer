/*************************************************
ClassName:   MainWindow
Description: 服务器控制中心
Date:        2019.6
Author:      Sepuer
E-mail:      ucun@qq.com
*************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QThread>
#include <QButtonGroup>
#include "Def.h"
#include "LogRecord.h"
#include "SvrEngine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //读取配置
    void initConf();
    //启用日志
    void runLog();
    //启动服务器
    void runSvr();

signals:
    void sign_startLog();
    void sign_stopLog();
public slots:
    void slot_output(QObject *pSock, QByteArray baData);
    void slot_count(int nType);

private:
    Ui::MainWindow *ui;

    SVR_CONF    m_tSvrConf;        //服务器配置
    LogRecord*  m_pLog;
    QThread*    m_pLogThd;
    SvrEngine   m_svrEngine;

    QButtonGroup    m_bgOutput;
    int         m_nConnSum;
    int         m_nOnline;
    int         m_nRecvSum;
};

#endif // MAINWINDOW_H
