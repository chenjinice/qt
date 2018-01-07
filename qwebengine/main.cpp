#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

    QMessageBox::about(0,"aaa","haode");
    QMessageBox::information(0,"title","bbbb");

    return a.exec();
}
