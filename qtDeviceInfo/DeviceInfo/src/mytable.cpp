#include <QHeaderView>
#include <QtAlgorithms>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QDebug>
#include "mytable.h"
#include "mytableitem.h"
#include "myclient.h"
#include "setting.h"
#include "udpthread.h"


MyTable::MyTable()
{
    qRegisterMetaType<UiCmdData>("UiCmdData");
    qRegisterMetaType<ToUiData>("ToUiData");
    m_header  = getTestItemsUi();
    m_ipIndex = m_header.indexOf(CI_IP);
    m_setRtcIndex = m_header.indexOf(CI_RTCSET);
    int cols = m_header.count();
    this->createMenu();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setAlternatingRowColors(true);
    this->setColumnCount(cols);
    this->setRowCount(0);
    this->setHorizontalHeaderLabels(m_header);
    this->horizontalHeader()->setStyleSheet("QHeaderView{border:0;border-bottom:1px solid #000;}");
    this->horizontalHeader()->setStretchLastSection(true);
//    this ->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(m_header.indexOf(CI_LTEVRX), QHeaderView::ResizeToContents);
    this->horizontalHeader()->setSectionResizeMode(m_ipIndex, QHeaderView::ResizeToContents);
    this->horizontalHeader()->setSectionResizeMode(m_setRtcIndex, QHeaderView::ResizeToContents);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for(int i=0;i<m_header.length();i++){
        bool flag = Setting::ins()->getHide(m_header.at(i));
        if(flag)this->setColumnHidden(i,true);
    }

    connect(this,&MyTable::customContextMenuRequested,this,&MyTable::showContextMenu);
}

MyTable::~MyTable()
{
    UiCmdData data;
    data.all = true;
    data.cmd = UI_DELETE;
    emit uiCmd(data);

    int rows = this->rowCount();
    int cols = this->columnCount();
    for (int i=0;i<rows;i++) {
        for(int m=0;m<cols;m++){
            QTableWidgetItem *item = this->item(i,m);
            if(item)delete item;
        }
    }
    this->clear();
}

void MyTable::addIp(QString ip)
{
    int col = m_header.count();
    int row = this->rowCount();
    this->insertRow(row);
    for(int m=0;m<col;m++){
        MyTableItem *item = new MyTableItem(this);
        if(m == m_ipIndex){
           item->setParam(ip);
        }
        this->setItem(row,m,item);
    }
    if(Setting::ins()->getAutoSort()){
        this->sortByIp();
    }
}

void MyTable::connectAll()
{
    UiCmdData data;
    data.all = true;
    data.cmd = UI_CONNECT;
    emit uiCmd(data);
}

void MyTable::clearResult()
{
    int rows = this->rowCount();
    int cols = this->columnCount();
    for (int i=0;i<rows;i++) {
        for(int m=0;m<cols;m++){
            if(m == m_ipIndex)continue;
            QTableWidgetItem *item = this->item(i,m);
            item->setIcon(QIcon());
            item->setText("");
        }
    }
}

void MyTable::sortByIp()
{
    this->sortByColumn(m_ipIndex,Qt::AscendingOrder);
}

void MyTable::hideItem(QString item, bool flag)
{
    int column = m_header.indexOf(item);
    this->setColumnHidden(column,flag);
}

void MyTable::createMenu()
{
    m_menu          = new QMenu(this);
    m_con           = new QAction(tr("连接"));
    m_disCon        = new QAction(tr("断开"));
    m_tf            = new QAction(tr("测tf"));
    m_eeprom        = new QAction(tr("测eeprom"));
    m_getRtc        = new QAction(tr("获取RTC"));
    m_setRtc        = new QAction(tr("更新RTC"));
    m_ledOn         = new QAction(tr("灯亮"));
    m_ledOff        = new QAction(tr("灯灭"));
    m_ledFlash      = new QAction(tr("灯闪烁"));
    m_del           = new QAction(tr("删除"));
    m_factory       = new QAction(tr("恢复出厂设置"));
    m_menu->addAction(m_tf);
    m_menu->addAction(m_eeprom);
//    m_menu->addAction(m_getRtc);
    m_menu->addAction(m_setRtc);
    m_menu->addAction(m_ledOn);
    m_menu->addAction(m_ledOff);
    m_menu->addAction(m_ledFlash);
    m_menu->addSeparator();
    m_menu->addAction(m_con);
    m_menu->addAction(m_disCon);
    m_menu->addAction(m_del);
    m_menu->addSeparator();
//    m_menu->addAction(m_factory);
//    m_factory->setEnabled(false);
    connect(m_tf,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_eeprom,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_getRtc,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_setRtc,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_ledOn,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_ledOff,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_ledFlash,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_con,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_disCon,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_del,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_factory,&QAction::triggered,this,&MyTable::ActionClicked);
}

void MyTable::showContextMenu(const QPoint &pos)
{
    qDebug() << pos;
    m_menu->exec(QCursor::pos());
}

void MyTable::getSelectedRows(QList<int> &l)
{
    const QList<QTableWidgetItem*> &items= this->selectedItems();
    int count= items.count();
    for(int i=0;i<count;i++){
        int row = this->row(items[i]);
        if(l.indexOf(row) == -1)l.push_back(row);
    }
    std::sort(l.begin(),l.end());
}

void MyTable::ActionClicked()
{
    QAction *s = qobject_cast<QAction *>(sender());
    QList<int> l;
    UiCmdData d;
    this->getSelectedRows(l);
    for(int i=0;i<l.count();i++){
        int row = l[i];
        MyTableItem *item = dynamic_cast<MyTableItem *>(this->item(row,m_ipIndex));
        d.l.append(item->getClient());
    }
    if(s == m_con){d.cmd = UI_CONNECT;}
    else if(s == m_disCon){d.cmd = UI_DISCONNECT;}
    else if(s == m_del){d.cmd = UI_DELETE;this->del(l);}
    else if(s == m_tf){d.cmd = UI_SEND;d.str = CC_TF;}
    else if(s == m_eeprom){d.cmd = UI_SEND;d.str = CC_EEPROM;}
    else if(s == m_getRtc){d.cmd = UI_SEND;d.str = CC_GETRTC;}
    else if(s == m_setRtc){d.cmd = UI_SEND;d.str = CC_SETRTC;}
    else if(s == m_ledOn){d.cmd = UI_SEND;d.str = CC_LEDON;}
    else if(s == m_ledOff){d.cmd = UI_SEND;d.str = CC_LEDOFF;}
    else if(s == m_ledFlash){d.cmd = UI_SEND;d.str = CC_LEDFLASH;}
    else if(s == m_factory){d.cmd = UI_SEND;d.str = CC_FACTORY;}
    emit uiCmd(d);
}

void MyTable::del(QList<int> &l)
{
    int row_count = l.count();
    int col_count = m_header.count();
    for(int i=row_count-1;i>=0;i--){
        int row = l[i];
        for(int j=0;j<col_count;j++){
            MyTableItem *item = dynamic_cast<MyTableItem *>(this->item(row,j));
            if(item){
                MyClient *client = item->getClient();
                if(client)UdpThread::ins()->del(client->ip());
                delete item;
            }
        }
        this->removeRow(row);
    }
}

void MyTable::showData(ToUiData d)
{
    MyClient *s = qobject_cast<MyClient *>(sender());
    int count = this->rowCount();
    int row = -1;
    for(int i=0;i<count;i++){
        MyTableItem *item = dynamic_cast<MyTableItem *>(this->item(i,m_ipIndex));
        if(item->getClient() == s){
            if(d.stateChanged)item->setConnectIcon(d.state);
            row = i;
        }
    }
    if(row == -1)return;

    int l_count = d.l.count();
    for(int m=0;m<l_count;m++){
        const ItemData &idata = d.l[m];
        int col = idata.column;
        QString text;
        MyTableItem *item = dynamic_cast<MyTableItem *>(this->item(row,col));
        if(item == nullptr)continue;
        if(idata.hasFlag)item->setBoolIcon(idata.flag);
        uint err = item->getErr();
        if(!idata.text.isEmpty())text+=idata.text + " ";
        if((err > 0) && (col!=m_setRtcIndex) ){
            text += "<" + QString::number(err) + ">";
        }
        item->setText(text);
    }
}

