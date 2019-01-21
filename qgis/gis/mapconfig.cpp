#include "mapconfig.h"
#include <QFileInfo>

MapConfig::MapConfig(QString &path)
{
    m_path = path;
    m_baseName = QFileInfo(path).completeBaseName();
    m_isEnabled = true;
}

MapConfig::~MapConfig()
{

}

QString &MapConfig::baseName()
{
    return m_baseName;
}

QString &MapConfig::path()
{
    return m_path;
}

bool MapConfig::isEnabled()
{
    return m_isEnabled;
}


