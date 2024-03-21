#include <QWebView>
#include <QWebFrame>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QThread>
#include "myview.h"
#include "mykit.h"


MyView::MyView(QWidget *parent) : QWidget(parent)
{
    this->init();
}

MyView::~MyView()
{
    delete m_kit;
}

void MyView::js(QString &cmd)
{
    if(!m_ready)return;
    m_web->page()->mainFrame()->evaluateJavaScript(cmd);
}

void MyView::init()
{
    m_ready  = false;
    m_layout = new QVBoxLayout;
    m_web = new QWebView;
    m_kit = new MyKit;

    m_layout->setMargin(0);
    m_layout->addWidget(m_web);
    this->setLayout(m_layout);

    m_web->load( QUrl("file:///"+QFileInfo("html/index.html").absoluteFilePath()) );
    connect(m_web->page()->mainFrame(),&QWebFrame::javaScriptWindowObjectCleared,this,&MyView::jsObjectCleared);
    connect(m_kit,&MyKit::mapUpdate,this,&MyView::getMapUpdate);
}

void MyView::jsObjectCleared()
{
    m_web->page()->mainFrame()->addToJavaScriptWindowObject("chen",m_kit);
    m_ready = true;
}

void MyView::getMapUpdate()
{
    QString cmd("init_map()");
    this->js(cmd);
}

void MyView::jsonParse(QString file_path)
{
    QString cmd = QString("json_parse(\"%1\")").arg(file_path);
    this->js(cmd);
}

void MyView::geoJson(QString file_path)
{
    QString cmd = QString("geo_json(\"%1\")").arg(file_path);
    this->js(cmd);
}

void MyView::addMarker()
{
    QString cmd = QString("qt_add()");
    this->js(cmd);
}

void MyView::addMarkerAt(double lat, double lng)
{
    QString lat_str = QString::number(lat,'f',7);
    QString lng_str = QString::number(lng,'f',7);
    QString cmd = QString("qt_addat(%1,%2)").arg(lat_str).arg(lng_str);
    this->js(cmd);
}

void MyView::goTo(double lat, double lng)
{
    QString lat_str = QString::number(lat,'f',7);
    QString lng_str = QString::number(lng,'f',7);
    QString cmd = QString("qt_go(%1,%2)").arg(lat_str).arg(lng_str);
    this->js(cmd);
}

void MyView::showObstacle(QString str)
{
    QString cmd = QString("qt_show_obs(\'%1\')").arg(str);
    this->js(cmd);
}

void MyView::clearMap()
{
    QString cmd = QString("qt_clear()");
    this->js(cmd);
}


