#include <QDebug>
#include <QTimer>
#include <QTcpSocket>
#include <QDir>
#include <QDateTime>
#include <QFile>
#include <synchapi.h>
#include <QApplication>
#include "myclient.h"
#include "mytable.h"
#include "setting.h"


MyClient::MyClient(QString ip, uint16_t port)
{
    m_connected     = false;
    m_ip            = ip;
    m_port          = port;
    m_thread        = new QThread;
    m_socket        = nullptr;
    m_timer         = nullptr;
    m_monitor       = nullptr;
    m_items         = getTestItems();

    this->moveToThread(m_thread);
    connect(m_thread,&QThread::started,this,&MyClient::threadRun);
    connect(m_thread,&QThread::finished,this,&MyClient::deleteConnection);

    m_thread->start();
}

MyClient::~MyClient()
{
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
}

QString MyClient::ip()
{
    return m_ip;
}

uint16_t MyClient::port()
{
    return m_port;
}

void MyClient::threadRun()
{
    m_timer = new QTimer;
    m_monitor = new QTimer;
    m_socket = new QTcpSocket;
    m_timer->setInterval(C_RETIME);
    m_monitor->setInterval(C_MOTIME);
    m_monitor->start();
    connect(m_timer,&QTimer::timeout,this,&MyClient::sockConnectHost);
    connect(m_monitor,&QTimer::timeout,this,&MyClient::monitorTimeout);
    connect(m_socket,&QTcpSocket::connected,this,&MyClient::sockConnected);
    connect(m_socket,&QTcpSocket::disconnected,this,&MyClient::sockDisconnected);
    connect(m_socket,&QTcpSocket::readyRead,this,&MyClient::sockReadData);

    if(Setting::ins()->getMode()){
        this->sockConnectHost();
    }
}

void MyClient::monitorTimeout()
{
    if(!m_connected)return;
    qint64 secs = m_lastTime.secsTo(QDateTime::currentDateTime());
    if(secs > C_MAXTIME){
        m_socket->disconnectFromHost();
        m_socket->abort();
    }
}

void MyClient::sockConnectHost()
{
    if(!m_connected){
        m_socket->disconnectFromHost();
        m_socket->abort();
        m_socket->connectToHost(m_ip,m_port,QTcpSocket::ReadWrite);
        m_socket->waitForConnected(500);
        if(!m_connected && m_timer){
            m_timer->start();
        }
    }
}

void MyClient::sockDisconnectHost()
{
    if(m_connected){
        m_socket->disconnectFromHost();
        m_socket->abort();
    }
    if(m_timer)m_timer->stop();
}

void MyClient::sockConnected()
{
    m_lastTime = QDateTime::currentDateTime();
    m_connected = true;
    ToUiData d;
    d.stateChanged = true;
    d.state = true;
    emit toUi(d);
    if(m_timer)m_timer->stop();
    this->send(CC_ALL);
}

void MyClient::sockDisconnected()
{
    m_connected = false;
    ToUiData d;
    d.stateChanged = true;
    d.state = false;
    emit toUi(d);
    if(m_timer)m_timer->start();
}

void MyClient::deleteConnection()
{
    if(m_timer){
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if(m_monitor){
        m_monitor->stop();
        delete m_monitor;
        m_monitor = nullptr;
    }
    if(m_socket){
        m_socket->disconnectFromHost();
        m_socket->abort();
        delete m_socket;
        m_socket = nullptr;
    }
}

void MyClient::send(const char *cmd)
{
    if(cmd == nullptr)return;
    if(m_connected){
        m_socket->write(cmd,strlen(cmd));
    }
}

void MyClient::getUiCmd(UiCmdData d)
{
    int index = d.l.indexOf(this);
    if((!d.all) && (index == -1))return;
    switch (d.cmd) {
    case UI_CONNECT:
        this->sockConnectHost();
        break;
    case UI_DISCONNECT:
        this->sockDisconnectHost();
        break;
    case UI_SEND:
        this->send(d.str);
        break;
    case UI_DELETE:
        this->deleteConnection();
        break;
    default:
        break;
    }
}

void MyClient::sockReadData()
{
    m_lastTime = QDateTime::currentDateTime();
    QByteArray array = m_socket->readAll();
    QString str = array;
    this->saveLog(str);
    this->parseData(str);
}

void MyClient::parseData(QString &str)
{
    QString pre1 = "-----";
    QString pre2 = "=================";
    ToUiData data;
    QStringList lines = str.remove("\r").split("\n");
    int line_count = lines.count();
    for(int i=0;i<line_count;i++){
        if(lines[i].indexOf(pre1) == 0){
            QString str = lines[i].mid(pre1.length());
            this->parsePre1(str,data);
        }else if(lines[i].indexOf(pre2) == 0){
            QString str = lines[i].mid(pre2.length());
            this->parsePre2(str,data);
        }
    }
    if(data.l.count() > 0)emit toUi(data);
}

bool MyClient::parsePre1(QString &str, ToUiData &d)
{
    bool ret = false;
    bool value = false;
    ItemData idata;
    idata.hasFlag = true;
    QStringList array = str.replace(QRegExp("[\\s]+")," ").split(" ");
    if(array.length() < 2)return ret;
    QString result = array[0].mid(0,array[0].indexOf("."));
    QString item   = array[1].mid(0,array[1].indexOf("."));
    int index = m_items.indexOf(item);
    if(index == -1)return ret;
    if(result.compare(C_OK,Qt::CaseInsensitive) == 0)value = true;
    idata.column = index;
    idata.flag = value;

    if(item == CI_4G){
        QStringList arr = str.split("=");
        if(arr.count() > 1)idata.text = arr[1]+" ms";
    }else if(item == CI_LTEVRX) {
         idata.text = this->getLtevrxText(str);
    }else if(item == CI_RADAR){
        if(array.length() > 2)idata.text = array[2] + "目标";
    }else if((item == CI_RTCGET) || (item == CI_RTCSET)){
        int count = array.length()-2;
        QString text;
        for(int i=2;i<count;i++){text += array[i];if(i < count -1)text += " ";}
        idata.text = text;
        bool flag = false;
        if(count >= 7){
            QString time_str = array[6]+" "+array[3]+" "+array[4]+" "+array[5];
            flag = this->checkTime(time_str);
        }
        idata.flag = flag;
    }
    d.l.append(idata);
    return true;
}

bool MyClient::parsePre2(QString &str, ToUiData &d)
{
    bool ret = false;
    int index= -1;
    QStringList array = str.split(" ");
    if(array.length() < 2)return ret;

    index = m_items.indexOf(CI_TIME);
    if(index != -1){
        ItemData idata;
        idata.column = index;
        if(array.length() > 2)idata.text += array[2];
        if(array.length() > 3)idata.text +=" " + array[3];
        d.l.append(idata);
        ret = true;
    }
    index = m_items.indexOf(CI_TMP1);
    if(index != -1){
        ItemData idata;
        idata.column = index;
        double temp1 = array[1].toInt()/1000.0;
        idata.text = QString::number(temp1,'f',3);
        d.l.append(idata);
        ret = true;
    }
    index = m_items.indexOf(CI_TMP2);
    if(index != -1){
        ItemData idata;
        idata.column = index;
        double temp2 = array[0].toInt()/1000.0;
        idata.text = QString::number(temp2,'f',3);
        d.l.append(idata);
        ret = true;
    }
    return true;
}

QString MyClient::getLtevrxText(QString &str)
{
    QString arr = "arr:", ct = "ct:" , text;
    int arr_index = str.indexOf(arr);
    int d_index = str.indexOf(",");
    int ct_index =str.indexOf(ct);

    int len = d_index-arr_index;
    if((d_index!=-1)&&(arr_index!=-1)&&(len>0)){
        text += str.mid(arr_index,len).trimmed()+",";
    }
    if(ct_index!=-1){
        text += str.mid(ct_index).trimmed();
    }
    return text;
}

bool MyClient::checkTime(QString &str)
{
    bool flag = false;
    QLocale locale = QLocale::English;
    QDateTime d = locale.toDateTime(str,"yyyy MMM dd hh:mm:ss");
    d.setTimeSpec(Qt::UTC);
    QDateTime current = QDateTime::currentDateTime().toUTC();
    qint64 seconds = qAbs(d.secsTo(current));
    if(seconds < 120)flag = true;
    return flag;
}

void MyClient::saveLog(QString &str)
{
    static int log_size = C_LOGSIZE;
    QString dirct = "log";
    QDir dir;
    if(!dir.exists(dirct))dir.mkpath(dirct);
    QString path = dirct + "/" + m_ip + ".log";
    QFile file(path);

    QFileInfo info(path);
    if(info.size() > log_size){
        QString old_path = dirct + "/" + m_ip + "____old.log";
        QFile   old_file(old_path);
        old_file.remove();
        file.rename(old_path);
    }

    QString log ;
    bool ret = file.open(QIODevice::Append | QIODevice::Text);
    if(!ret)return;
    log += QDateTime::currentDateTime().toString("[yyyy.MM.dd hh:mm:ss]:\r\n");
    log += str;
    file.write(log.toUtf8());
    file.close();
}

