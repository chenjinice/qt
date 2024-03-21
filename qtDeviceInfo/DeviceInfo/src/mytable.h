#ifndef MYTABLE_H
#define MYTABLE_H


#include <QTableWidget>
#include <QMap>
#include "myclient.h"
#include "common.h"
#include "mytableitem.h"


class MyTable : public QTableWidget
{
    Q_OBJECT
public:
    MyTable();
    ~MyTable();
    void addIp(QString ip);
    void connectAll();
    void clearResult();
    void sortByIp();
    void hideItem(QString item,bool flag);

private:
    int                    m_ipIndex;
    int                    m_setRtcIndex;
    QMenu                * m_menu;
    QAction              * m_con;
    QAction              * m_disCon;
    QAction              * m_tf;
    QAction              * m_eeprom;
    QAction              * m_getRtc;
    QAction              * m_setRtc;
    QAction              * m_ledOn;
    QAction              * m_ledOff;
    QAction              * m_ledFlash;
    QAction              * m_del;
    QAction              * m_factory;
    QStringList            m_header;

    void createMenu();
    void showContextMenu(const QPoint &pos);
    void getSelectedRows(QList<int> &l);
    void ActionClicked();
    void del(QList<int> &l);

public slots:
    void showData(ToUiData d);


signals:
    void uiCmd(UiCmdData d);
};

#endif // MYTABLE_H
