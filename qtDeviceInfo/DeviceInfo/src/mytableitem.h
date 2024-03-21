#ifndef MYTABLEITEM_H
#define MYTABLEITEM_H


class MyTable;
class MyClient;

#include <QObject>
#include <QTableWidgetItem>


class MyTableItem : public QTableWidgetItem
{
public:
    MyTableItem(MyTable *ui);
    ~MyTableItem();
    void setParam(QString &ip);
    void setBoolIcon(bool flag);
    void setConnectIcon(bool flag);
    uint getErr();
    MyClient *getClient();
    bool operator < (const QTableWidgetItem &other) const;

private:
    MyTable     * m_ui;
    MyClient    * m_client;
    uint          m_sortId;
    uint          m_err;

};

#endif // MYTABLEITEM_H
