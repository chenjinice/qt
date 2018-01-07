#include "playerdata.h"
#include <QStringList>
#include <QString>

PlayerData::PlayerData()
{
    m_colorName = "#000000";
    m_nowIndex = 0;
    m_list = new QStringList();
}

PlayerData::~PlayerData()
{
    delete m_list;
}

void PlayerData::insertValue(const QString &time, const QString &lng, const QString &lat)
{
    QString t = time;
    t.replace(time.indexOf('T'),1,' ');
    QString str = QString("%1,%2,%3").arg(t).arg(lng).arg(lat);
    m_list->append(str);
}

void PlayerData::setId(QString id)
{
    m_id = id;
}

QString PlayerData::getId()
{
    return m_id;
}

void PlayerData::clear()
{
    m_nowIndex = 0;
    m_id = nullptr;
    m_list->clear();
}

int PlayerData::length()
{
    return m_list->length();
}

int PlayerData::nowIndex()
{
    return m_nowIndex;
}

void PlayerData::pp()
{
    m_nowIndex++;
}

void PlayerData::resetIndex()
{
    m_nowIndex = 0;
}

QString PlayerData::valueAtIndex(int index)
{
    return m_list->at(index);
}

QString PlayerData::time()
{
    if(m_nowIndex > m_list->length()-1)return nullptr;
    return m_list->at(m_nowIndex).split(",")[0];
}

QString PlayerData::lng()
{
    if(m_nowIndex > m_list->length()-1)return nullptr;
    return m_list->at(m_nowIndex).split(",")[1];
}

QString PlayerData::lat()
{
    if(m_nowIndex > m_list->length()-1)return nullptr;
    return m_list->at(m_nowIndex).split(",")[2];
}

void PlayerData::setColorName(const QString &colorName)
{
    m_colorName = colorName;
}

QString PlayerData::colorName()
{
    return m_colorName;
}


