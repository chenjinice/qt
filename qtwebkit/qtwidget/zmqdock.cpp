#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "logdock.h"
#include "zmqdock.h"
#include "setting.h"
#include "zmq.h"
#include "vision.pb.h"
#include "fusion.pb.h"
#include "radar.pb.h"

using namespace vision;
using namespace fusionPackage;
using namespace radarPackage;

#define BUFFER_SIZE 10240

void *zmq_read_thread(void *param)
{
    // 线程结束时，自动释放资源
    pthread_detach(pthread_self());
    ZmqDock *p = (ZmqDock *)param;
    p->threadRun();
    return NULL;
}

ZmqDock::ZmqDock(QWidget *parent) : QDockWidget(parent)
{
    this->init();
    this->readSetting();
}

ZmqDock::ZmqDock(char *filter, QWidget *parent)
 : QDockWidget(parent)
{
    this->init();
    m_fixed = true;
    strcpy(m_filter,filter);
    m_filter_edit->setText(m_filter);
    m_filter_edit->setDisabled(true);
    this->setWindowTitle(m_filter);
    this->readSetting();
}

ZmqDock::~ZmqDock()
{
    this->stop_clicked();
}

void ZmqDock::init()
{
    int i=0;
    memset(m_filter,0,sizeof(m_filter));
    m_subscriber = NULL;
    m_context = NULL;
    m_fixed = false;
    m_reading = false;

    this->setWindowTitle("zmq");
    this->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);

    QWidget *widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    QLabel *ip_label = new QLabel("ip");
    QLabel *port_label = new QLabel("port");
    QLabel *filter_label = new QLabel("filter");
    m_ip_edit = new QLineEdit("127.0.0.1");
    m_port_edit = new QLineEdit("12347");
    m_filter_edit = new QLineEdit("");
    m_start_button = new QPushButton("start");
    m_stop_button = new QPushButton("stop");
    layout->addWidget(ip_label,i,0,1,1);
    layout->addWidget(m_ip_edit,i++,1,1,1);
    layout->addWidget(port_label,i,0,1,1);
    layout->addWidget(m_port_edit,i++,1,1,1);
    layout->addWidget(filter_label,i,0,1,1);
    layout->addWidget(m_filter_edit,i++,1,1,1);
    layout->addWidget(m_start_button,i++,0,1,2);
    layout->addWidget(m_stop_button,i++,0,1,2);
    layout->setVerticalSpacing(2);
    widget->setMaximumSize(200,180);
    widget->setLayout(layout);
    this->setWidget(widget);
    this->disableWidget(false);

    connect(m_start_button,&QPushButton::clicked,this,&ZmqDock::start_clicked);
    connect(m_stop_button,&QPushButton::clicked,this,&ZmqDock::stop_clicked);
    connect(this,&ZmqDock::uiLog,LogDock::ins(),&LogDock::log);
}

bool ZmqDock::zmqStart(char *ip, int port)
{
    if(m_reading)return true;

    int ret;
    char sub_endpoint[50] = {0};
    sprintf(sub_endpoint,"tcp://%s:%d",ip,port);
    m_context = zmq_ctx_new();
    this->myLog(QString("sub %1[%2]").arg(sub_endpoint).arg(m_filter));
    m_subscriber = zmq_socket(m_context,ZMQ_SUB);
    ret = zmq_connect(m_subscriber,sub_endpoint);
    if(ret != 0){
        this->myLog( QString("zmq connect error:%1").arg( strerror(errno)) );
        return false;
    }
    ret = zmq_setsockopt(m_subscriber,ZMQ_SUBSCRIBE,m_filter,strlen(m_filter));
    if(ret != 0){
        this->myLog("zmq set filter error");
        return false;
    }
    pthread_t thread;
    pthread_create(&thread,NULL,zmq_read_thread,this);
    m_reading = true;
    return true;
}

void ZmqDock::zmq_stop()
{
    m_reading = false;
    if(m_context){
        zmq_ctx_destroy(m_context);
        m_context = NULL;
    }
}

void ZmqDock::start_clicked()
{
    QByteArray tmp_ip = m_ip_edit->text().toLatin1();
    QByteArray tmp_filter = m_filter_edit->text().toLatin1();
    int port = m_port_edit->text().toInt();
    char *ip = tmp_ip.data();
    strcpy(m_filter,tmp_filter.data());

    bool ret = this->zmqStart(ip,port);
    if(ret)this->disableWidget(true);

    this->saveSetting();
}

void ZmqDock::stop_clicked()
{
    this->zmq_stop();
    this->disableWidget(false);
}

void ZmqDock::disableWidget(bool flag)
{
    m_ip_edit->setDisabled(flag);
    m_port_edit->setDisabled(flag);
    m_start_button->setDisabled(flag);
    m_stop_button->setEnabled(flag);
    if(!m_fixed){
        m_filter_edit->setDisabled(flag);
    }
}

void ZmqDock::threadRun()
{
    uint8_t filter[40] = {0};
    uint8_t buffer[BUFFER_SIZE];
    int len;
    while (m_reading){
        memset(buffer,0,sizeof(buffer));
        if(strlen(m_filter) > 0)len = zmq_recv(m_subscriber,filter,sizeof(filter),0);
        len = zmq_recv(m_subscriber,buffer,sizeof(buffer),0);
        if(len == -1)continue;
        if(len > BUFFER_SIZE){
            this->myLog(QString("zmq:read size(%d) > buffer_size(%d)").arg(len).arg(BUFFER_SIZE));
            continue;
        }
        if(strcmp(m_filter,"vision") == 0){
            analysisVision(buffer,len);
        }else if(strcmp(m_filter,"fusion") == 0){
            analysisFusion(buffer,len);
        }else if(strcmp(m_filter,"radar") == 0){
            analysisRadar(buffer,len);
        }else{
            this->myLog(QString("zmq:no rules for %1").arg(m_filter));
        }
    }
    zmq_close(m_subscriber);
    m_subscriber = NULL;
}

void ZmqDock::readSetting()
{
    QString title = this->windowTitle();
    QString ip = Setting::ins()->value(title+"_ip").toString();
    QString port = Setting::ins()->value(title+"_port").toString();
    QString filter = Setting::ins()->value(title+"_filter").toString();

    if(!ip.isEmpty())m_ip_edit->setText(ip);
    if(!port.isEmpty())m_port_edit->setText(port);
    if(title == "zmq")m_filter_edit->setText(filter);
}

void ZmqDock::saveSetting()
{
    QString title = this->windowTitle();
    QString ip = m_ip_edit->text();
    QString port = m_port_edit->text();
    QString filter = m_filter_edit->text();

    Setting::ins()->setValue(title+"_ip",ip);
    Setting::ins()->setValue(title+"_port",port);
    if(title == "zmq")Setting::ins()->setValue(title+"_filter",filter);
}

void ZmqDock::myLog(const QString &str)
{
    emit uiLog(str);
}



// ------------------------ vision --------------------------------
void ZmqDock::analysisVision(uint8_t *buffer, int len)
{
    Base base;
    if(!base.ParseFromArray(buffer,len)){this->myLog(QString("vision:base unpack fail,len=%1").arg(len));return;}
    switch (base.id()) {
        case PEDESTRIAN_D:
        case TRAFFIC_STATUS:
            this->pedestrianFun(buffer,len);
            break;
        case DETECTOBJECTS:
            this->detectobjFun(buffer,len);
        default:
            break;
    }
}

void ZmqDock::pedestrianFun(uint8_t *buffer, int len)
{
    Crowd c;
    if( !c.ParseFromArray(buffer,len) ){this->myLog("vision:crowd unpack failed");return;}
    QJsonDocument doc;
    QJsonObject root;
    QJsonArray array;
    int count = c.pedestrian_size();
    for(int i=0;i<count;i++){
        const Pedestrian &p = c.pedestrian(i);
        QJsonObject obs;
        obs.insert("id",p.object_id());
        obs.insert("lng",p.lng());
        obs.insert("lat",p.lat());
        array.insert(i,obs);
        this->myLog(QString("%5 pedestrian[%1]:id=%2,lng=%3,lat=%4").arg(i).arg(p.object_id()).arg(p.lng()).arg(p.lat()).arg(m_filter));
    }
    root.insert("source",m_filter);
    root.insert("array",array);
    doc.setObject(root);
    QByteArray byte = doc.toJson(QJsonDocument::Compact);
    QString str(byte);
    emit show_position(str);
}

void ZmqDock::detectobjFun(uint8_t *buffer, int len)
{
    Detectobjects d;
    if( !d.ParseFromArray(buffer,len) ){this->myLog("vision:detect object unpack failed");return;}

    QJsonDocument doc;
    QJsonObject root;
    QJsonArray array;
    int count = d.object_size();
    for(int i=0;i<count;i++){
        const Detectobject &p = d.object(i);
        QJsonObject obs;
        obs.insert("id",p.object_id());
        obs.insert("lng",p.longitudinalx());
        obs.insert("lat",p.lateraly());
        array.insert(i,obs);
        this->myLog(QString("%5 object[%1]:id=%2,longitudinalx=%3,lateraly=%4").arg(i).arg(p.object_id()).arg(p.longitudinalx()).arg(p.lateraly()).arg(m_filter));
    }
    root.insert("source",m_filter);
    root.insert("array",array);
    doc.setObject(root);
    QByteArray byte = doc.toJson(QJsonDocument::Compact);
    QString str(byte);
    emit show_position(str);
}

// ------------------------ fusion --------------------------------
void ZmqDock::analysisFusion(uint8_t *buffer, int len)
{
    fusionMsg f;
    if(!f.ParseFromArray(buffer,len)){this->myLog(QString("fusion:msg unpack fail,len=%1").arg(len));return;}

    QJsonDocument doc;
    QJsonObject root;
    QJsonArray array;
    int count = f.msg_size();
    for(int i=0;i<count;i++){
        const fusionPackage::targetMessage &p = f.msg(i);
        QJsonObject obs;
        obs.insert("id",(int)p.id());
        obs.insert("lng",(int64_t)p.lon());
        obs.insert("lat",(int64_t)p.lat());
        array.insert(i,obs);
        this->myLog(QString("%5 msg[%1]:id=%2,lng=%3,lat=%4").arg(i).arg(p.id()).arg(p.lon()).arg(p.lat()).arg(m_filter));
    }
    root.insert("source",m_filter);
    root.insert("array",array);
    doc.setObject(root);
    QByteArray byte = doc.toJson(QJsonDocument::Compact);
    QString str(byte);
    emit show_position(str);
}


// ------------------------ radar --------------------------------
void ZmqDock::analysisRadar(uint8_t *buffer, int len)
{
    radarMsg m;
    if(!m.ParseFromArray(buffer,len)){this->myLog(QString("radar:msg unpack fail,len=%1").arg(len));return;}

    QJsonDocument doc;
    QJsonObject root;
    QJsonArray array;
    int count = m.msg_size();
    for(int i=0;i<count;i++){
        const radarPackage::targetMessage &p = m.msg(i);
        QJsonObject obs;
        obs.insert("id",(int)p.id());
        obs.insert("lat",p.longitudinalx());
        obs.insert("lng",p.lateraly());
        array.insert(i,obs);
        this->myLog(QString("%5 msg[%1]:id=%2,lng=%3,lat=%4").arg(i).arg(p.id()).arg(p.longitudinalx()).arg(p.lateraly()).arg(m_filter));
    }
    root.insert("source",m_filter);
    root.insert("array",array);
    doc.setObject(root);
    QByteArray byte = doc.toJson(QJsonDocument::Compact);
    QString str(byte);
    emit show_position(str);
}




