#include "table/planwidget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlanWidget w;
    w.show();
    return a.exec();
}
