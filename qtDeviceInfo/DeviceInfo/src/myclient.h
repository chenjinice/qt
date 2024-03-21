#ifndef MYCLIENT_H
#define MYCLIENT_H

class QThread;
class QTcpSocket;
class QTimer;

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QDateTime>
#include <QMap>
#include "common.h"


class MyClient : public QObject
{
    Q_OBJECT
public:
    MyClient(QString ip, uint16_t port);
    ~MyClient();
    QString ip();
    uint16_t port();

private:
    QStringList    m_items;
    bool           m_connected;
    QString        m_ip;
    uint16_t       m_port;
    QTimer       * m_timer;
    QTimer       * m_monitor;
    QThread      * m_thread;
    QTcpSocket   * m_socket;
    QDateTime      m_lastTime;

    void threadRun();
    void monitorTimeout();
    void sockConnectHost();
    void sockDisconnectHost();
    void sockConnected();
    void sockDisconnected();
    void deleteConnection();
    void send(const char * cmd);
    void sockReadData();
    void parseData(QString &str);
    bool parsePre1(QString &str, ToUiData &d);
    bool parsePre2(QString &str, ToUiData &d);
    QString getLtevrxText(QString &str);
    bool checkTime(QString &str);
    void saveLog(QString &str);

public slots:
    void getUiCmd(UiCmdData d);

signals:
    void toUi(ToUiData d);

};

#endif // MYCLIENT_H
