#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class MyView;
class QLineEdit;
class Online;
class QTimer;
class PlayerData;

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    MyView *m_view;
    QLineEdit *m_selectLine;
    Online    *m_onLine;
    QTimer    *m_timer;
    PlayerData *m_play;

public slots:
    void pushButton_clicked();
    void selectSoldier_clicked();
    void deleteSoldierButton_clicked();
    void deleteAllSoldierButton_clicked();
    void path_clicked();
    void deleteAllPath_clicked();
    void SeeAll_clicked();
    void getVisible_clicked();

    void solderId_slot(const QString &solderId);
    void recvVisibleID_slot(const QStringList &ids);


    void timer_slot();
};

#endif // MAINWINDOW_H
