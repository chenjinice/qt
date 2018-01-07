#include "online.h"
#include <QtSql>
#include <QDebug>

Online::Online()
{
    this->setData("a");
}

Online::~Online()
{
    m_list.clear();
}

QList<QMap<QString, QString> > &Online::getList()
{
    return m_list;
}

void Online::setData(const QString &str)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.0.1");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("uplus");
    db.setDatabaseName("test");

    if(!db.open()){
        qDebug() << db.lastError();
        return;
    }

//    QSqlQuery q;
    QString sql = QString("insert into test values(22,'%1','f',50)").arg(str);
    qDebug() << sql;

//    bool success = q.exec(sql);
//    if(!success){
//        qDebug() << q.lastError();
//        return;
//    }
    QSqlQuery q = db.exec(sql);
    qDebug() << q.record().isEmpty();

    db.close();

}

void Online::getDBData()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.0.1");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("uplus");
    db.setDatabaseName("soldiers");

    if(!db.open()){
        qDebug() << db.lastError();
        return;
    }

    QSqlQuery q;
    QString str = QString("SELECT a.*, b.f_soldiertypename, c.f_rankname, d.f_levelname, e.f_postname, f.f_sexname,"\
                          " g.f_nationname, h.f_politicsname, i.f_idtypename, j.f_unitname,j.f_unitshortname from t_soldier a "\
                          " LEFT JOIN t_soldiertype b on a.f_soldiertype=b.f_soldiertype"\
                          " LEFT JOIN t_rank c on a.f_rank=c.f_rank"\
                          " LEFT JOIN t_level d on a.f_level=d.f_level"\
                          " LEFT JOIN t_post e on a.f_post=e.f_post"\
                          " LEFT JOIN t_sex f on a.f_sex=f.f_sex"\
                          " LEFT JOIN t_nation g on a.f_nation=g.f_nation"\
                          " LEFT JOIN t_politics h on a.f_politics=h.f_politics"\
                          " LEFT JOIN t_idtype i on a.f_idtype=i.f_idtype "\
                          " LEFT JOIN t_unit j on a.f_unitid=j.f_unitid "\
                          " where a.f_online = 1");

    bool success = q.exec(str);
    if(!success){
        qDebug() << q.lastError();
        return;
    }

    QSqlRecord rec = q.record();
    //把数据库在线状态为1的项，加入 m_olList
    while (q.next()) {
        QMap<QString,QString> map;
        for(int i=0;i<rec.count();i++){
            map.insert(rec.fieldName(i),q.value(i).toString());
        }
        m_list.append(map);
    }
    db.close();
}
