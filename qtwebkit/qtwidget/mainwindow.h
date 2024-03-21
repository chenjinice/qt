#ifndef MAINWINDOW_H
#define MAINWINDOW_H


class QMdiArea;
class MyView;
class QTextEdit;
class QAction;
class LogDock;
class ZmqDock;


#include <QMainWindow>
#include <QHostInfo>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    MyView      *m_webview;
    ZmqDock     *m_fusion_dock;
    ZmqDock     *m_vision_dock;
    ZmqDock     *m_radar_dock;
    ZmqDock     *m_zmq_dock;
    LogDock     *m_log_dock;

    void setMenu();
    void setToolBar();

private slots:
    void new_triggered();
    void json_triggered();
    void geo_triggered();
    void addAt_triggered();
    void goTo_triggered();
    void aboutMe_triggered();
};

#endif // MAINWINDOW_H
