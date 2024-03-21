#include <QDebug>
#include "setting.h"


Setting *Setting::m_instance = NULL;


Setting *Setting::ins()
{
    if(m_instance == NULL){
        m_instance = new Setting;
    }
    return m_instance;
}

Setting::Setting() : QSettings("CIDI","mapTool")
{

}

