#ifndef MAINWINDOW_H
#define MAINWINDOW_H


class MyTable;
class QLabel;
class QVBoxLayout;
class QCheckBox;
class UdpThread;


#include <QMainWindow>
#include "common.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QVBoxLayout  * m_layout;
    QLabel       * m_labelState;
    UdpThread    * m_udpThread;
    MyTable      * m_table;

    void createMenu();
    void createCentralWidget();
    void createCombo();
    void createButton();
    void aboutMe();

public slots:
    void udpState(bool flag);
    void clearClicked();
    void sortBoxChanged(int value);
};

#endif // MAINWINDOW_H
