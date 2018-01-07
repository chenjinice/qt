#ifndef ITEM_H
#define ITEM_H

#include <QTableWidgetItem>

class Item : public QTableWidgetItem
{
public:
    Item();
    ~Item();

    void rememberSpan(int rowSpan ,int colSpan);
    int rowSpan();
    int colSpan();

private:
    int m_rowSpan;
    int m_colSpan;
};

#endif // ITEM_H
