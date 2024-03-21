#include <QThread>
#include <QUdpSocket>
#include <QTimer>
#include <QDebug>
#include "udpthread.h"
#include "mainwindow.h"
#include "setting.h"

UdpThread  * UdpThread::m_instance = nullptr;


UdpThread *UdpThread::ins()
{
    if(m_instance == nullptr){
        m_instance = new UdpThread();
    }
    return  m_instance;
}

void UdpThread::startThread(QString &ip)
{
    if(!m_thread->isRunning()){
        m_ip = ip;
        m_thread->start();
    }
}

void UdpThread::add(const QString &ip)
{
    m_mutex.lock();
    if(m_list.indexOf(ip) == -1){
        m_list.append(ip);
        emit getIp(ip);
    }
    m_mutex.unlock();
}

void UdpThread::del(const QString &ip)
{
    m_mutex.lock();
    m_list.removeAll(ip);
    m_mutex.unlock();
}

UdpThread::UdpThread()
{
    m_ready     = false;
    m_udp       = nullptr;
    m_timer     = nullptr;
    m_ip        = "127.0.0.1";
    m_thread    = new QThread;
    this->moveToThread(m_thread);

    connect(m_thread,&QThread::started,this,&UdpThread::threadRun);
    connect(m_thread,&QThread::finished,this,&UdpThread::threadFinish);
}

UdpThread::~UdpThread()
{
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
}

void UdpThread::setIp(QString ip)
{
    if(m_udp == nullptr)return;
    Setting::ins()->setCard(ip);
    m_ip = ip;
    this->closeUdp();
    this->bindUdp();
}

void UdpThread::threadRun()
{
    m_udp   = new QUdpSocket;
    m_timer = new QTimer;
    m_timer->setInterval(C_UDPTIME);

    connect(m_udp  ,&QUdpSocket::readyRead, this, &UdpThread::readData);
    connect(m_timer,&QTimer::timeout      , this, &UdpThread::bindUdp);
    m_timer->start();
    this->bindUdp();
}

void UdpThread::threadFinish()
{
    if(m_timer){
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if(m_udp){
        this->closeUdp();
        delete m_udp;
        m_udp = nullptr;
    }
}

void UdpThread::bindUdp()
{
    if(!m_ready){
        m_ready = m_udp->bind(QHostAddress(m_ip), C_UDPPORT);
        emit bindState(m_ready);
    }
    if(m_ready){
        QByteArray data = "OR-TEST";
        m_udp->writeDatagram(data, QHostAddress::Broadcast, 12303);
    }
}

void UdpThread::closeUdp()
{
    m_ready = false;
    m_udp->close();
}

void UdpThread::readData()
{
    QByteArray array;
    QHostAddress address;
    int len = static_cast<int>(m_udp->pendingDatagramSize());
    array.resize(len);
    m_udp->readDatagram(array.data(), array.size(),&address);

    QString ip = address.toString();
    this->add(ip);
}
