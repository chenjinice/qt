#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include <QWebView>
#include <QTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QToolBar>
#include <QDialog>
#include <QApplication>
#include "mainwindow.h"
#include "myview.h"
#include "logdock.h"
#include "zmqdock.h"
#include "mydialog.h"


static QString g_title = "Map.2019.11.22";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_webview = new MyView;
    m_log_dock = LogDock::ins();
    m_fusion_dock = new ZmqDock((char *)"fusion");
    m_vision_dock = new ZmqDock((char *)"vision");
    m_radar_dock  = new ZmqDock((char *)"radar");
    m_zmq_dock = new ZmqDock;
    this->addDockWidget(Qt::LeftDockWidgetArea,m_radar_dock);
    this->addDockWidget(Qt::LeftDockWidgetArea,m_fusion_dock);
    this->addDockWidget(Qt::LeftDockWidgetArea,m_vision_dock);
    this->addDockWidget(Qt::LeftDockWidgetArea,m_zmq_dock);
    this->addDockWidget(Qt::BottomDockWidgetArea,m_log_dock);

//    m_fusion_dock->close();
    m_vision_dock->close();
    m_zmq_dock->close();
    m_log_dock->close();

    this->setMenu();
    this->setToolBar();
    this->setCentralWidget(m_webview);
    this->setWindowTitle(g_title);

    connect(m_radar_dock,&ZmqDock::show_position,m_webview,&MyView::showObstacle);
    connect(m_fusion_dock,&ZmqDock::show_position,m_webview,&MyView::showObstacle);
    connect(m_vision_dock,&ZmqDock::show_position,m_webview,&MyView::showObstacle);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setMenu()
{
    QMenuBar *menu_bar = new QMenuBar;

    QMenu *file_menu = new QMenu("File");
    QAction *new_a = new QAction("??",this);
    QAction *json_a = new QAction("open json",this);
    QAction *geo_a = new QAction("geo json",this);
    file_menu->addAction(json_a);
    file_menu->addAction(geo_a);
    file_menu->addAction(new_a);
    menu_bar->addMenu(file_menu);

    QMenu *about_menu = new QMenu("About");
    QAction *aboutme_a = new QAction("Me",this);
    QAction *qt_a = new QAction("Qt",this);
    about_menu->addAction(aboutme_a);
    about_menu->addAction(qt_a);
    menu_bar->addMenu(about_menu);

    this->setMenuBar(menu_bar);
    connect(new_a,&QAction::triggered,this,&MainWindow::new_triggered);
    connect(json_a,&QAction::triggered,this,&MainWindow::json_triggered);
    connect(geo_a,&QAction::triggered,this,&MainWindow::geo_triggered);
    connect(aboutme_a,&QAction::triggered,this,&MainWindow::aboutMe_triggered);
    connect(qt_a,&QAction::triggered,qApp,&QApplication::aboutQt);
}

void MainWindow::setToolBar()
{
    QToolBar *toolbar = this->addToolBar("toolbar");
    QAction *add = new QAction("add",this);
    QAction *addat = new QAction("addat",this);
    QAction *go = new QAction("go",this);
    QAction *clear = new QAction("clear",this);
    toolbar->addAction(add);
    toolbar->addAction(addat);
    toolbar->addAction(go);
    toolbar->addAction(clear);
    toolbar->setMovable(false);
    toolbar->close();

    connect(add,&QAction::triggered,m_webview,&MyView::addMarker);
    connect(clear,&QAction::triggered,m_webview,&MyView::clearMap);
    connect(addat,&QAction::triggered,this,&MainWindow::addAt_triggered);
    connect(go,&QAction::triggered,this,&MainWindow::goTo_triggered);
}

void MainWindow::new_triggered()
{
}

void MainWindow::json_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("json"),"./",tr("json(*json);;""All(*)"));
    if(filename.isEmpty())return;
    m_webview->jsonParse(filename);
    this->setWindowTitle(g_title + "    --" + filename);
}

void MainWindow::geo_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("json"),"./",tr("json(*json);;""All(*)"));
    if(filename.isEmpty())return;
    m_webview->geoJson(filename);
    this->setWindowTitle(g_title + "    --" + filename);
}

void MainWindow::addAt_triggered()
{
    double lat,lng;
    if( MyDialog::getPoint(lat,lng) ){
        m_webview->addMarkerAt(lat,lng);
    }
}

void MainWindow::goTo_triggered()
{
    double lat,lng;
    if( MyDialog::getPoint(lat,lng) ){
        m_webview->goTo(lat,lng);
    }
}

void MainWindow::aboutMe_triggered()
{
    MyDialog::aboutMe();
}




