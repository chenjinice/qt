#ifndef VISIONDOCK_H
#define VISIONDOCK_H

class QPushButton;
class QComboBox;
class QLineEdit;

#include <stdint.h>
#include <QDockWidget>

class ZmqDock : public QDockWidget
{
    Q_OBJECT
public:
    explicit ZmqDock(QWidget *parent = 0);
    ZmqDock(char * filter,QWidget *parent = 0);
    ~ZmqDock();

private:
    void init();
    bool zmqStart(char *ip, int port);
    void zmq_stop();
    void disableWidget(bool flag);
    void threadRun();
    void readSetting();
    void saveSetting();
    void myLog(const QString &str);

    void analysisVision(uint8_t *buffer,int len);
    void pedestrianFun(uint8_t *buffer,int len);
    void detectobjFun(uint8_t *buffer,int len);

    void analysisFusion(uint8_t *buffer,int len);
    void analysisRadar(uint8_t *buffer,int len);

    friend void *zmq_read_thread(void *param);

    bool         m_fixed;
    QLineEdit   *m_ip_edit;
    QLineEdit   *m_port_edit;
    QLineEdit   *m_filter_edit;
    QPushButton *m_start_button;
    QPushButton *m_stop_button;

    bool        m_reading;
    char        m_filter[100];
    void        *m_context;
    void        *m_subscriber;

public slots:
    void start_clicked();
    void stop_clicked();

signals:
    void show_position(QString);
    void uiLog(QString str);
};

#endif // VISIONDOCK_H
