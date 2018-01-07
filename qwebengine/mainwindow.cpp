#include "mainwindow.h"
#include "webview/myview.h"
#include "sql/online.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QDockWidget>
#include <QDir>
#include <QTime>
#include <QColor>
#include <QLineEdit>
#include <QTimer>
#include "dialog/playerdata.h"
#include <QDateTime>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *mainWidget = new QWidget;
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    mainWidget->setLayout(hboxLayout);
    hboxLayout->setMargin(0);
    this->setCentralWidget(mainWidget);

    m_play =new PlayerData();

    // 创建测试用的按钮
    QDockWidget *dockwidget = new QDockWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QWidget  *widget = new QWidget;
    widget->setLayout(leftLayout);
    dockwidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dockwidget->setWidget(widget);
    this->addDockWidget(Qt::LeftDockWidgetArea,dockwidget);

    QPushButton *addButton = new QPushButton("updatePosition");
    QPushButton *deleteSoldierButton = new QPushButton("deleteSoldier");
    QPushButton *deleteAllSoldierButton = new QPushButton("deleteAllSoldier");
    QPushButton *pathButton = new QPushButton("showPath");
    QPushButton *deletePathButton = new QPushButton("deleteAllPath");
    QPushButton *getVisibleButton = new QPushButton("getVisible");
    QPushButton *seeAllButton = new QPushButton("seeAll");
    m_selectLine = new QLineEdit;
    m_selectLine->setToolTip("填要选中的士兵id");
    leftLayout->addWidget(addButton);
    leftLayout->addWidget(m_selectLine);
    leftLayout->addWidget(deleteSoldierButton);
    leftLayout->addWidget(deleteAllSoldierButton);
    leftLayout->addWidget(pathButton);
    leftLayout->addWidget(deletePathButton);
    leftLayout->addWidget(getVisibleButton);
    leftLayout->addWidget(seeAllButton);

    //  在线成员
    m_onLine = new Online();

    // 加载电子地图模块
//    QString path = "file:///"+QDir::currentPath()+"/baidu_map/index.html";
    QString path = "file:////mnt/hgfs/share/baidu_map/index.html";
//    QString path = "file:////mnt/hgfs/share/google_map/index.html";
//    QString path = "file:////mnt/hgfs/share/baidu_map/main.html";
    m_view = new MyView(path,this);
    hboxLayout->addWidget(m_view);


    connect(addButton,&QPushButton::clicked,this,&MainWindow::pushButton_clicked);
    connect(m_selectLine,&QLineEdit::returnPressed,this,&MainWindow::selectSoldier_clicked);
    connect(deleteSoldierButton,&QPushButton::clicked,this,&MainWindow::deleteSoldierButton_clicked);
    connect(deleteAllSoldierButton,&QPushButton::clicked,this,&MainWindow::deleteAllSoldierButton_clicked);
    connect(pathButton,&QPushButton::clicked,this,&MainWindow::path_clicked);
    connect(deletePathButton,&QPushButton::clicked,this,&MainWindow::deleteAllPath_clicked);
    connect(getVisibleButton,&QPushButton::clicked,this,&MainWindow::getVisible_clicked);
    connect(seeAllButton,&QPushButton::clicked,this,&MainWindow::SeeAll_clicked);

    // 通过订阅 交互类 的信号，也可以获得html页面点击的士兵id
    connect(m_view->webObj(),&WebObject::clicked_soldierId,this,&MainWindow::solderId_slot);
    connect(m_view->webObj(),&WebObject::recvVisibleID,this,&MainWindow::recvVisibleID_slot);

    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&MainWindow::timer_slot);

}

MainWindow::~MainWindow()
{
    delete m_timer;
}

void MainWindow::pushButton_clicked()
{
    QList<QString> shorNames;
    shorNames.append("一");
    shorNames.append("二");
    shorNames.append("三");
    shorNames.append("四");
    shorNames.append("五");
    shorNames.append("六");
    shorNames.append("七");
    shorNames.append("八");
    shorNames.append("九");
    QTime time = QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    QJsonObject text;
    text.insert("type","1");
    QJsonArray jsonArray;
    for(int i=0;i<100;i++){
        QJsonObject json;
        json.insert("f_soldierid",QString::number(i));        // 士兵 id
        int type = i%2+1;
        json.insert("f_soldiertype",QString::number(type));   // type
        json.insert("f_soldiername",QString::number(i)+"号"); // 姓名

        double lng = qrand()%500/100.00+113.00;
        json.insert("f_curlongitude",QString::number(lng,10,4)); // 经度

        double lat = qrand()%500/100.00+37.00;
        json.insert("f_curlatitude",QString::number(lat,10,4));  // 纬度

        json.insert("f_unitinfo","无");                          //  info
        QString icon = m_view->getIconName(i%2+1,shorNames[i%8],QColor(0,255,0));  //图标
        if(!icon.isEmpty()){
            json.insert("icon",icon);
        }
        jsonArray.append(json);
    }
    text.insert("soldiers",jsonArray);

    m_view->webObj()->updatePosition(text);
//        qDebug() << jsonArray;

//    m_timer->start(2000);
}

void MainWindow::selectSoldier_clicked()
{
    QString id = m_selectLine->text();
    if(id.isEmpty())return;

//    m_view->webObj()->selectSoldier(id,true,7);

    id.replace("'","&apos;");
    qDebug() << id;
//    m_onLine->setData(id);
}

void MainWindow::deleteSoldierButton_clicked()
{
    QJsonArray jsonArray;

    QJsonObject json;
    // 士兵 id
    json.insert("f_soldierid","1");

    QJsonObject json1;
    json1.insert("f_soldierid","2");

    jsonArray.append(json);
    jsonArray.append(json1);

    m_view->webObj()->deleteSoldiers(jsonArray);
//    qDebug() << jsonArray;
}

void MainWindow::deleteAllSoldierButton_clicked()
{
    m_view->setParent(0);
    m_view->show();
    m_view->webObj()->deleteAllSoldiers();
}

QJsonObject lastJoson ;
void MainWindow::path_clicked()
{
    QJsonObject json;

//    int id = qrand()%8;
    int id = 1;
    // id
    json.insert("f_soldierid",QString::number(id));
    // 线的颜色
    QString r  = QString::number(qrand()%256);
    QString g  = QString::number(qrand()%256);
    QString b  = QString::number(qrand()%256);
    QString color = "rgb("+r+","+g+","+b+")";
    json.insert("color","#ff33aa");
//    json.insert("color",color);

    // 经纬度的数组
    QJsonArray jarray;
    QJsonObject parr;
    //经度
    double lng = qrand()%6000/100.00+80.00;
    parr.insert("f_curlongitude",lng);
    //纬度
    double lat = qrand()%3000/100.00+20.00;
    parr.insert("f_curlatitude",lat);

    QJsonObject parr1;
    lng = qrand()%6000/100.00+80.00;
    lat = qrand()%3000/100.00+20.00;
    parr1.insert("f_curlongitude",lng);
    parr1.insert("f_curlatitude",lat);

    QJsonObject parr2;
    lng = qrand()%6000/100.00+80.00;
    lat = qrand()%3000/100.00+20.00;
    parr2.insert("f_curlongitude",lng);
    parr2.insert("f_curlatitude",lat);

    QJsonObject parr3;
    lng = qrand()%6000/100.00+80.00;
    lat = qrand()%3000/100.00+20.00;
    parr3.insert("f_curlongitude",lng);
    parr3.insert("f_curlatitude",lat);

    jarray.append(lastJoson);
    jarray.append(parr);
    lastJoson = parr;
//    jarray.append(parr);
//    jarray.append(parr1);
//    jarray.append(parr2);
//    jarray.append(parr3);

    json.insert("path",jarray);

    m_view->webObj()->showPath(json);
    //    qDebug() << json;
}

void MainWindow::deleteAllPath_clicked()
{
    m_view->webObj()->deleteAllPath();
}

void MainWindow::SeeAll_clicked()
{
    m_view->webObj()->zoomToSeeAll();
}

void MainWindow::getVisible_clicked()
{
    m_view->webObj()->getVisible();
}

void MainWindow::solderId_slot(const QString &solderId)
{
    qDebug() << "mainwindow : " << solderId;
}

void MainWindow::recvVisibleID_slot(const QStringList &ids)
{
    qDebug() << ids;
}

void MainWindow::timer_slot()
{
    static double s_lng = 116.404;
    static double s_lat = 39.915;
    static int s_count = 0;

    QJsonObject json;
    int id = 1;
    json.insert("f_soldierid",QString::number(id));
    json.insert("color","#ff33aa");

    // 经纬度的数组
    QJsonArray jarray;
    QJsonObject parr;
    parr.insert("f_curlongitude",s_lng);
    parr.insert("f_curlatitude",s_lat);

    QJsonObject parr1;
    double lng = qrand()%6000/100.00+80.00;
    double lat = qrand()%3000/100.00+20.00;
    parr1.insert("f_curlongitude",lng);
    parr1.insert("f_curlatitude",lat);
    s_lng = lng;
    s_lat = lat;

    jarray.append(parr);
    jarray.append(parr1);

    json.insert("path",jarray);

    m_view->webObj()->showPath(json);

    s_count++;
    if(s_count > 100){
        s_count = 0;
        m_view->webObj()->deleteAllPath();
    }
    if(s_count % 10 == 0)qDebug() << s_count;

}




