#ifndef COMMON_H
#define COMMON_H

class MyClient;

#include <QString>
#include <QStringList>


#define C_LOGSIZE       1024*1024*50


#define C_TCPPORT       12301
#define C_UDPPORT       12302
#define C_UDPTIME       3000     // udp绑定端口失败时，重新绑定间隔（也是广播数据间隔），毫秒
#define C_RETIME        5000     // tcp重连定时器间隔 ,毫秒
#define C_MOTIME        5000     // 监控定时器间隔 ,毫秒
#define C_MAXTIME       30       // 多长时间没收到数据就断开 ,秒


// 发送指令
#define CC_ALL          "ttuv2x all"
#define CC_TF           "ttfcar"
#define CC_EEPROM       "tteeprom"
#define CC_GETRTC       "rtc-get"
#define CC_SETRTC       "rtc-update"
#define CC_LEDOFF       "ttled 0"
#define CC_LEDON        "ttled 1"
#define CC_LEDFLASH     "ttled 2"
#define CC_FACTORY      "refactory force"


#define C_OK            "ok"
// 测试项目
#define CI_IP           "ip"
#define CI_LTEVRX       "ltev_rx"
#define CI_USB          "usb-com"
#define CI_GPS          "gps"
#define CI_RS485        "rs485"
#define CI_CAN          "can"
#define CI_RADAR        "radar"
#define CI_4G           "4G"
#define CI_RJ45         "rj45"
#define CI_WIFI         "wifi"
#define CI_TF           "tf"
#define CI_EEPROM       "eeprom"
#define CI_RTCGET       "rtc_get"
#define CI_RTCSET       "rtc_set"
#define CI_TMP1         "主板温度"
#define CI_TMP2         "CPU温度"
#define CI_TIME         "设备时间"
#define CI_OBD          "obd"
#define CI_RS232        "rs232"
#define CI_RS422        "rs422"
#define CI_OTG          "otg"
#define CI_SECU         "secu"


enum UICMD{
    UI_NONE,
    UI_CONNECT,
    UI_DISCONNECT,
    UI_SEND,
    UI_DELETE,
};


uint ipToNumber(QString &ip);
QStringList getSelfIp();
QStringList getTestItems();
QStringList getTestItemsUi();



class UiCmdData{
public:
    UiCmdData(){
        all = false;
        cmd = UI_NONE;
        str = nullptr;
    }
    bool               all;
    UICMD              cmd;
    const char       * str;
    QList<MyClient *>  l;
};


class ItemData{
public:
    ItemData(){
        column = -1;
        hasFlag = false;
        flag    = false;
    }
    int     column;
    bool    hasFlag;
    bool    flag;
    QString text;
};

class ToUiData{
public:
    ToUiData(){
        stateChanged = false;
        state = false;
    }
    bool  stateChanged;
    bool  state;
    QList<ItemData> l;
};

#endif // COMMON_H
