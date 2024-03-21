#ifndef MYVIEW_H
#define MYVIEW_H

class MyKit;
class QWebView;
class QVBoxLayout;
class QWebFrame;

#include <QWidget>
#include <QList>

class MyView : public QWidget
{
    Q_OBJECT
public:
    explicit MyView(QWidget *parent = 0);
    ~MyView();



private:
    void init();
    void js(QString &cmd);

    bool         m_ready;
    MyKit       *m_kit;
    QWebView    *m_web;
    QVBoxLayout *m_layout;

private slots:
    void jsObjectCleared();
    void getMapUpdate();

public slots:
    void jsonParse(QString file_path);
    void geoJson(QString file_path);
    void addMarker();
    void addMarkerAt(double lat,double lng);
    void goTo(double lat,double lng);
    void showObstacle(QString str);
    void clearMap();

};

#endif // MYVIEW_H
