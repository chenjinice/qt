#include <QNetworkInterface>
#include <QDebug>
#include <QString>
#include "common.h"


static QStringList g_items;
static QStringList g_ui_items;


uint ipToNumber(QString &ip)
{
    uint id = 0;
    QStringList nums = ip.split(".");
    if(nums.count() == 4){
        id = nums[0].toUInt()*256*256*256 + nums[1].toUInt()*256*256 + nums[2].toUInt()*256 + nums[3].toUInt();
    }else{
        qDebug() << "ipToId error : count != 4";
    }
    return id;
}


QStringList getSelfIp()
{
    QStringList value;
    QList<QHostAddress> l = QNetworkInterface::allAddresses();
    int n = l.size();
    for (int i = 0; i < n; i++){
        if (l.at(i) != QHostAddress::LocalHost && l.at(i).toIPv4Address()) {
            value <<  l.at(i).toString();
        }
    }
    value.append("127.0.0.1");
    return value;
}


QStringList getTestItems()
{
    if(g_items.count() == 0){
        g_items << CI_IP  << CI_LTEVRX << CI_USB    << CI_GPS  << CI_RS485 << CI_CAN  << CI_RADAR << CI_4G  << CI_RJ45  << CI_WIFI
                << CI_TF  << CI_EEPROM << CI_RTCSET << CI_TMP1 << CI_TMP2  << CI_TIME << CI_OBD << CI_RS232 << CI_RS422
                << CI_OTG << CI_SECU ;
    }
    return g_items;
}

QStringList getTestItemsUi()
{
    if(g_ui_items.count() == 0){
        g_ui_items = getTestItems();
    }
    return g_ui_items;
}


