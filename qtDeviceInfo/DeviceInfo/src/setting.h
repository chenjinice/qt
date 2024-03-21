#ifndef SETTING_H
#define SETTING_H

#include <QSettings>


#define CS_MODE     "mode"
#define CS_CARD     "card"
#define CS_SORT     "autoSort"
#define CS_IP1      "ip1"
#define CS_IP2      "ip2"
#define CS_IP3      "ip3"
#define CS_IP4      "ip4"
#define CS_TO4      "to4"
#define CS_HIDE     "hide_"


class Setting : public QSettings
{
public:
    static Setting *ins();

    void            setCard(QString str);
    QString         getCard();
    void            setMode(bool flag);
    bool            getMode();
    void            setAutoSort(bool flag);
    bool            getAutoSort();
    void            setHide(const QString &item, bool flag);
    bool            getHide(const QString &item);

private:
    static Setting * m_instance;

    Setting();
};



#endif // SETTING_H
