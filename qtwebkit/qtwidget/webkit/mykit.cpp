#include <unistd.h>
#include <math.h>
#include <QFile>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThread>
#include <QMutex>
#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QApplication>
#include <QDateTime>
#include <QProcess>
#include <QUrlQuery>
#include "mykit.h"
#include "logdock.h"


MyKit::MyKit()
{
    m_ready = false;
    m_google = false;
    m_manager = NULL;
    m_request = NULL;
    m_thread = new QThread;
    m_mutex = new QMutex;
    m_img.status = 0;

    this->moveToThread(m_thread);
    connect(m_thread,&QThread::started,this,&MyKit::threadRun);
    connect(this,&MyKit::uiLog,LogDock::ins(),&LogDock::log);
    m_thread->start();
}

MyKit::~MyKit()
{
    m_ready = false;
    m_thread->quit();
    m_thread->wait();
    delete m_manager;
    delete m_request;
    delete m_thread;
//    m_thread->deleteLater();
    delete m_mutex;
    m_list.clear();
}

void MyKit::threadRun()
{
    m_ready = true;
    if(!m_manager)m_manager = new QNetworkAccessManager;
    if(!m_request)m_request = new QNetworkRequest;
    connect(m_manager,&QNetworkAccessManager::finished,this,&MyKit::downloadFinish);

    this->connectToGoogle();
    while(m_ready){
        // while循环里边不用这个的话，不能接收QT的信号
        QApplication::processEvents();
        if(!m_google){
            if(!this->connectToGoogle()){sleep(5);continue;}
        }
        if(m_list.length() == 0){sleep(1);continue;}
        if(m_img.status == 0){
            m_img = m_list[0];
            QString url_name = QString("http://mt1.google.com/vt/lyrs=s&x=%1&y=%2&z=%3").arg(m_img.x).arg(m_img.y).arg(m_img.z);
            m_request->setUrl(QUrl(url_name));
            m_manager->get(*m_request);
        }else{
            if(m_img.status-- <= -300){
                this->listRemove(m_img);
                this->connectToGoogle();
            }
        }
        usleep(20*1000);
    }
}

bool MyKit::connectToGoogle()
{
    QProcess process;
    process.start("ping -s 1 -c 1 www.google.com");
    process.waitForStarted();
    if( !process.waitForFinished(3000) ){
        process.kill();
        process.waitForFinished(100);
    }
    QString str = process.readAll();
    if(str.indexOf("ttl") == -1){
        qDebug() << "connect to google failed !!!";
        m_google = false;
    }else{
        qDebug() << "connect to google success !!!";
        m_google = true;
    }
    return m_google;
}

void MyKit::listRemove(MyKit::ImgInfo &m)
{
    m_mutex->lock();
    int len = m_list.length(),i;
    for(i=len-1;i>=0;i--){
        ImgInfo *p = &m_list[i];
        if( (p->x==m.x) &&(p->y==m.y)&&(p->z==m.z))m_list.removeAt(i);
    }
    m_img.status = 0;
    m_mutex->unlock();
}

bool MyKit::listFind(MyKit::ImgInfo &m)
{
    bool ret = false;
    int len = m_list.length(),i;
    for(i=0;i<len;i++){
        ImgInfo *p = &m_list[i];
        if( (p->x==m.x) &&(p->y==m.y)&&(p->z==m.z)){
            ret = true;
            break;
        }
    }
    return ret;
}

void MyKit::listAdd(MyKit::Range &x, MyKit::Range &y, int z)
{
    int max = 100;
    if(!m_google)return;
    m_mutex->lock();
    for(int i=x.min;i<=x.max;i++){
        for(int j=y.min;j<=y.max;j++){
            QString file_name = QString("./html/map/%1/%2/%3.jpg").arg(z).arg(i).arg(j);
            QFile file(file_name);
            if(file.exists())continue;

            ImgInfo m;
            m.x = i;
            m.y = j;
            m.z = z;
            m.status = -1;
            if(this->listFind(m))continue;
            m_list.prepend(m);

            if(m_list.length() > max)m_list.pop_back();
        }
    }
    m_mutex->unlock();
}

void MyKit::listClear()
{
    m_mutex->lock();
    m_list.clear();
    m_mutex->unlock();
}

double MyKit::moduleOperation(double num, int min, int max)
{
    if( (num <= max) && (num >= min) )return num;
    // 将num转华为min到max范围内的数
    int m = (max-min);
    int n = floor( (num-min)/m );
    return num-n*m;
}

void MyKit::getX(double lng1, double lng2, int z, MyKit::Range &r)
{
    lng1 = this->moduleOperation(lng1,-180,180);
    lng2 = this->moduleOperation(lng2,-180,180);
    int max = pow(2,z);
    int x1 = pow(2,z-1)*(lng1/180.0+1);
    int x2 = pow(2,z-1)*(lng2/180.0+1);
    x1 %= max;
    x2 %= max;
    if(x1>x2){
        r.max = x1;
        r.min = x2;
    }else{
        r.max = x2;
        r.min = x1;
    }
}

void MyKit::getY(double lat1, double lat2, int z, MyKit::Range &r)
{
    int lat_max = 85;
    lat1 = this->moduleOperation(lat1,-90,90);
    lat2 = this->moduleOperation(lat2,-90,90);
    if(lat1 > lat_max)lat1 = lat_max;
    if(lat2 > lat_max)lat2 = lat_max;
    if(lat1 < -lat_max)lat1 = -lat_max;
    if(lat2 < -lat_max)lat2 = -lat_max;
    double temp1 = tan(M_PI*lat1/180.0)+1.0/cos(M_PI*lat1/180.0);
    double temp2 = tan(M_PI*lat2/180.0)+1.0/cos(M_PI*lat2/180.0);
    int y1 = pow(2,z-1)*(1-log(temp1)/M_PI);
    int y2 = pow(2,z-1)*(1-log(temp2)/M_PI);
    if(y1>y2){
        r.max = y1;
        r.min = y2;
    }else{
        r.max = y2;
        r.min = y1;
    }
}

void MyKit::sendUpdateSignal()
{
    static QDateTime s_tv = QDateTime::currentDateTime();
    QDateTime now = QDateTime::currentDateTime();
    qint64 s = s_tv.secsTo(now);
    if( (s > 1) || (m_list.length() == 0) ){
        emit mapUpdate();
        s_tv = now;
    }
}

void MyKit::myLog(const QString &str)
{
    emit uiLog(str);
}

void MyKit::downloadFinish(QNetworkReply *reply)
{
    QString url = reply->url().toString();
    QUrlQuery query(url);
    ImgInfo m ; memset(&m,0,sizeof(ImgInfo));
    m.x = query.queryItemValue("x").toInt();
    m.y = query.queryItemValue("y").toInt();
    m.z = query.queryItemValue("z").toInt();
    this->listRemove(m);
    if (reply->error() == QNetworkReply::NoError){
        qDebug() << "download ok :" << url;
        QByteArray bytes = reply->readAll();
        QString file_path = QString("./html/map/%1/%2/").arg(m.z).arg(m.x);
        QString file_name = QString("./html/map/%1/%2/%3.jpg").arg(m.z).arg(m.x).arg(m.y);
        QDir dir;
        dir.mkpath(file_path);
        QFile file(file_name);
        file.open(QIODevice::WriteOnly);
        file.write(bytes,bytes.length());
        file.close();
        this->sendUpdateSignal();
    }else{
        qDebug() << "download fail :" << url << " , " << reply->error();
    }
}

void MyKit::get_bounds(double lat1, double lat2, double lng1, double lng2,int zoom)
{
//    qDebug() << lat1 << "," << lat2 << "," << lng1 << "," << lng2 << "," << zoom << "---------";
    Range x_range,y_range;
    getX(lng1,lng2,zoom,x_range);
    getY(lat1,lat2,zoom,y_range);
    this->listAdd(x_range,y_range,zoom);
    qDebug("x(%d,%d) , y(%d,%d) , z(%d)",x_range.min,x_range.max,y_range.min,y_range.max,zoom);
//    this->mylog(QString("x(%1,%2) , y(%3,%4) , z(%5)").arg(x_range.min).arg(x_range.max).arg(y_range.min).arg(y_range.max).arg(zoom));
}

