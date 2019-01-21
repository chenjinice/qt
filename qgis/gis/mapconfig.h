#ifndef MAPCONFIG_H
#define MAPCONFIG_H

#include <QString>

class MapConfig
{
public:
    MapConfig(QString &path);
    ~MapConfig();

    QString &baseName();
    QString &path();
    bool    isEnabled();

private:
    QString m_baseName;
    QString m_path;
    bool    m_isEnabled;

};

#endif // MAPCONFIG_H
