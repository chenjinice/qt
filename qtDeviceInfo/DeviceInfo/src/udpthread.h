#ifndef UDPTHREAD_H
#define UDPTHREAD_H

class QThread;
class QUdpSocket;
class MainWindow;
class QTimer;

#include <QObject>
#include <QMutex>
#include "common.h"

class UdpThread : public QObject
{
    Q_OBJECT
public:
    static UdpThread *ins();
    ~UdpThread();
    void startThread(QString &ip);
    void add(const QString &ip);
    void del(const QString &ip);

private:
    UdpThread();
    void threadRun();
    void threadFinish();
    void bindUdp();
    void closeUdp();
    void readData();

    static UdpThread * m_instance;
    bool               m_ready;
    QString            m_ip;
    QThread          * m_thread;
    QUdpSocket       * m_udp;
    QTimer           * m_timer;
    QList<QString>     m_list;
    QMutex             m_mutex;

public slots:
    void setIp(QString ip);

signals:
    void bindState(bool flag);
    void getIp(QString ip);
};

#endif // UDPTHREAD_H
