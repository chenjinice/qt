#ifndef SETTING_H
#define SETTING_H

#include <QSettings>

class Setting : public QSettings
{

public:
    static Setting *ins();

private:
    Setting();
    static Setting *m_instance;
};

#endif // SETTING_H
