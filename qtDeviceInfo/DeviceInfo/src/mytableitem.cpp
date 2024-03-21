#include <QDebug>
#include "mytableitem.h"
#include "myclient.h"
#include "mytable.h"
#include "setting.h"


MyTableItem::MyTableItem(MyTable *ui)
{
    m_ui = ui;
    m_sortId = 0;
    m_err = 0;
    m_client = nullptr;
    this->setTextAlignment(Qt::AlignCenter);
}

MyTableItem::~MyTableItem()
{
    if(m_client)delete m_client;
}

void MyTableItem::setParam(QString &ip)
{
    if(m_client)return;

    m_sortId = ipToNumber(ip);
    uint16_t port = C_TCPPORT;
    this->setText(ip);
    m_client = new MyClient(ip,port);
    QObject::connect(m_ui,&MyTable::uiCmd,m_client,&MyClient::getUiCmd);
    QObject::connect(m_client,&MyClient::toUi,m_ui,&MyTable::showData);
}

void MyTableItem::setBoolIcon(bool flag)
{
    static QIcon yes(":/icon/image/yes.png");
    static QIcon no(":/icon/image/no.png");
    if(flag){
        this->setIcon(yes);
    }else{
        m_err++;
        this->setIcon(no);
    }
}

void MyTableItem::setConnectIcon(bool flag)
{
    static QIcon c_yes(":/icon/image/connect_yes.png");
    static QIcon c_no(":/icon/image/connect_no.png");
    if(flag)this->setIcon(c_yes);
    else this->setIcon(c_no);
}

uint MyTableItem::getErr()
{
    return  m_err;
}

MyClient *MyTableItem::getClient()
{
    return m_client;
}

bool MyTableItem::operator <(const QTableWidgetItem &other) const
{
    const MyTableItem *item =  dynamic_cast<const MyTableItem *>(&other);
    uint that_id = item->m_sortId;
    return m_sortId < that_id;
}

