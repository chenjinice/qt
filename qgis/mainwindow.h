#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class GisWindow;

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    GisWindow *m_gisWindow;

};

#endif // MAINWINDOW_H
