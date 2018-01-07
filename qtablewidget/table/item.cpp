#include "item.h"
#include <QDebug>

Item::Item()
    :m_rowSpan(1),
     m_colSpan(1)
{

}

Item::~Item()
{

}

void Item::rememberSpan(int rowSpan, int colSpan)
{
    m_rowSpan = rowSpan;
    m_colSpan = colSpan;
}

int Item::rowSpan()
{
    return m_rowSpan;
}

int Item::colSpan()
{
    return m_colSpan;
}
