#ifndef PLAYERDATA_H
#define PLAYERDATA_H

class QStringList;

#include <QString>

class PlayerData
{
public:
    PlayerData();
    ~PlayerData();
    void insertValue(const QString &time,const QString &lng ,const QString &lat);
    void setId(QString id);
    QString getId();
    void clear();
    int length();
    int nowIndex();
    void pp();
    void resetIndex();
    QString valueAtIndex(int index);
    QString time();
    QString lng();
    QString lat();
    void setColorName(const QString &colorName);
    QString colorName();

private:
    QStringList *m_list;
    QString     m_colorName;
    QString     m_id;
    int         m_nowIndex;
};

#endif // PLAYERDATA_H
