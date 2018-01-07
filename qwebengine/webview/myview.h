/*
 * 电子地图 widget
 *
 * chen jin
 *
 */

#ifndef MYVIEW_H
#define MYVIEW_H

class QWebEngineView;
class WebObject;
class QJsonArray;
class QJsonObject;

#include <QWidget>
#include "webobject.h"

class MyView : public QWidget
{
    Q_OBJECT
public:
    //path:html页面的路径
    explicit MyView(QString &path, QWidget *parent = nullptr);
    ~MyView();

    //与js交互的对象的指针
    WebObject *webObj();
    // 生成的icon图片，返回图片路径
    QString getIconName(int nType , QString shortName, QColor color);
    // 画图函数
    bool generatePngFile(QString sOriginPngFileName,QString sDesPngFileName,
                            QString strText,int nFontSize,int nTextPosX,int nTextPosY,QColor clrText,QColor clrTextShadow,
                            int nLineWidth,int nLineHeight,int nLinePosX,int nLinePosY,QColor clrLine,QColor clrLineShadow);
    QString MD5String(const QString str);
    // 返回底图的路径名
    QString getOriginPngName(int type , bool isSel);
    // 返回目标图的路径名
    QString getDestPngName(int type , bool isSel , QString shortName,QColor color);

private:
    QWebEngineView *m_webView;
    //与html交互需要的类
    WebObject      *m_webObject;
    //html页面路径
    QString         m_path;
    //页面是否加载完成变量
    bool            m_isLoadFinished;

signals:

protected slots:
    void setReady();

public slots:
};

#endif // MYVIEW_H
