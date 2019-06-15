#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pLog = nullptr;
    m_pLogThd = nullptr;
    m_nConnSum = 0;
    m_nOnline = 0;
    m_nRecvSum = 0;

    connect(&m_svrEngine, &SvrEngine::sign_output, this, &MainWindow::slot_output);
    connect(&m_svrEngine, &SvrEngine::sign_count, this, &MainWindow::slot_count);

    m_bgOutput.addButton(ui->rbStr, 0);
    m_bgOutput.addButton(ui->rbHex, 1);

    initConf();
    runLog();
    runSvr();
}

MainWindow::~MainWindow()
{
    if(m_pLog != nullptr)
    {
        delete m_pLog;
        m_pLog = nullptr;
    }
    if(m_pLogThd != nullptr)
    {
        m_pLogThd->quit();
        m_pLogThd->wait();
        m_pLogThd->deleteLater();
        m_pLogThd = nullptr;
    }
    delete ui;
}

void MainWindow::initConf()
{
    QSettings setSvr(FILE_CONFIG, QSettings::IniFormat);

    m_tSvrConf.addrSvr = setSvr.value("Net/AddrSvr", "127.0.0.1").toString();
    m_tSvrConf.nPort = setSvr.value("Net/Port", 12345).toInt();
    m_tSvrConf.nNetThd = setSvr.value("Work/NetThd", 10).toInt();
    m_tSvrConf.bLog = setSvr.value("Sys/Log", 0).toBool();

    QString status;
    status.sprintf("IP: %s  PORT: %d", m_tSvrConf.addrSvr.toStdString().data(), m_tSvrConf.nPort);
    statusBar()->showMessage(status);
}

void MainWindow::runLog()
{
    if(!m_tSvrConf.bLog)
    {
        return;
    }
    m_pLog = LogRecord::getInstance();
    m_pLogThd = new QThread(this);
    connect(this, SIGNAL(sign_startLog()), m_pLog, SLOT(slot_startLog()));
    connect(this, SIGNAL(sign_stopLog()), m_pLog, SLOT(slot_stopLog()));

    m_pLogThd->start();
    m_pLog->moveToThread(m_pLogThd);

    emit sign_startLog();
}

void MainWindow::runSvr()
{
    m_svrEngine.svrStart(m_tSvrConf);
}

void MainWindow::slot_output(QObject *pSock, QByteArray baData)
{
    QString output;

    switch (m_bgOutput.checkedId())
    {
    case 0:
        output.sprintf("[%p]: %s", pSock, baData.toStdString().data());
        break;
    case 1:
        output.sprintf("[%p]: %s", pSock, baData.toHex().toStdString().data());
        break;
    }
    ui->pteOutput->appendPlainText(output);
    qDebug() << output;
    //qDebug() << qPrintable(output);

    slot_count(TP_RECV_ADD);
}

void MainWindow::slot_count(int nType)
{
    switch (nType)
    {
    case TP_CONN_ADD:
        m_nConnSum++;
        m_nOnline++;
        ui->lbConnSum->setNum(m_nConnSum);
        ui->lbOnline->setNum(m_nOnline);
        break;
    case TP_CONN_DEL:
        m_nOnline--;
        ui->lbOnline->setNum(m_nOnline);
        break;
    case TP_RECV_ADD:
        m_nRecvSum++;
        ui->lbRecvSum->setNum(m_nRecvSum);
        break;
    }
}
