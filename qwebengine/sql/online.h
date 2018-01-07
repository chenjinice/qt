#ifndef ONLINE_H
#define ONLINE_H


#define OnLineList QList<QMap<QString,QString>>

#include <QList>
#include <QMap>

class Online
{
public:
    Online();
    ~Online();
    OnLineList &getList();
    void setData(const QString &str);

private:
    OnLineList m_list;

    void getDBData();

};

#endif // ONLINE_H
