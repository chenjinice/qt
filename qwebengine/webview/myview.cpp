/*
 * 电子地图 widget
 *
 * chen jin
 *
 */

#define PATH_ORIGIN_PRE     "/baidu_map/styles/soldiersicon/origin/"
#define PATH_DEST_PRE       "/baidu_map/styles/soldiersicon/dest/"
#define PATH_COMMANDER      "commander"
#define PATH_SOLDIER        "soldier"
#define PATH_SELECT         "_select"
#define ICON_TYPE           ".png"

#include "myview.h"
#include "webobject.h"
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QString>

#include <QDir>
#include <QPixmap>
#include <QPainter>
#include <QCryptographicHash>

MyView::MyView(QString &path, QWidget *parent) : QWidget(parent)
{
    m_isLoadFinished = false;
    m_path = path;
    QVBoxLayout *vLayout = new QVBoxLayout;
    this->setLayout(vLayout);

    m_webView = new QWebEngineView;
    vLayout->addWidget(m_webView);

    // 与html的js交互需要QWebChannel
    m_webObject = new WebObject(this);
    QWebChannel *webchannel = new QWebChannel(this);
    webchannel->registerObject(QStringLiteral("webObject"),m_webObject);
    m_webView->page()->setWebChannel(webchannel);

    //加载html
    QUrl url(m_path);
    m_webView->load(url);
    //关联 html加载完成 的信号
    connect(m_webView,&QWebEngineView::loadFinished,this,&MyView::setReady);
}

MyView::~MyView()
{
}

WebObject *MyView::webObj()
{
    return m_webObject;
}

QString MyView::getIconName(int nType, QString shortName,QColor color)
{
    bool flag = true;
    bool isSel = true;

    //生成选中时的图片
    QString destSelName = this->getDestPngName(nType,isSel,shortName,color);
    if(!QFileInfo(destSelName).exists()){
        QString originName = this->getOriginPngName(nType,isSel);
        flag &= generatePngFile(originName,destSelName,shortName,10, 5,14,QColor(0,0,0,200),QColor(255,255,255),9,2,5,2,color,QColor(0,0,0));
    }

    //生成没选中时的图片
    isSel = false;
    QString destName = this->getDestPngName(nType,isSel,shortName,color);
    if(!QFileInfo(destName).exists()){
        QString originName = this->getOriginPngName(nType,isSel);
        flag &= generatePngFile(originName,destName,shortName,10, 5,14,QColor(0,0,0,200),QColor(255,255,255),9,2,5,2,color,QColor(0,0,0));
    }

    if(flag){
        return QFileInfo(destName).completeBaseName();
    }else {
        return "";
    }
}

bool MyView::generatePngFile(QString sOriginPngFileName, QString sDesPngFileName, QString strText, int nFontSize, int nTextPosX, int nTextPosY, QColor clrText, QColor clrTextShadow, int nLineWidth, int nLineHeight, int nLinePosX, int nLinePosY, QColor clrLine, QColor clrLineShadow)
{
    bool bRet=true;
    QPixmap imgSrc;
    if(!imgSrc.load(sOriginPngFileName))
    {
        return false;
    }

    QFont font;
    font.setPixelSize(nFontSize);

    QPainter p(&imgSrc);
    p.setFont(font);
    if(nLineHeight>0&&nLineWidth>0)
    {
        QPen penBorder(QBrush(clrLineShadow),nLineHeight+3);
        p.setPen(penBorder);
        p.drawLine(nLinePosX,nLinePosY-1,nLinePosX+nLineWidth-1,nLinePosY-1);
        QPen penLine(QBrush(clrLine),nLineHeight);
        p.setPen(penLine);
        p.drawLine(nLinePosX,nLinePosY,nLinePosX+nLineWidth,nLinePosY);
    }
    if(!strText.isEmpty())
    {
        QPainterPath pp(QPointF(nTextPosX,nTextPosX));
        pp.addText(nTextPosX, nTextPosY, font, strText);

        QPainterPathStroker pps;
        pps.setCapStyle(Qt::RoundCap);

        QPainterPath path;

        QColor clrShadow=clrTextShadow;
        int ics=4;
        for(int i=ics;i>0;i--)
        {
            pps.setWidth(i);
            path = pps.createStroke(pp).united(pp);//.simplified();//
            clrShadow.setAlpha(200-30*i);
            p.fillPath(path, clrShadow);
        }

        pps.setWidth(0);
        path = pps.createStroke(pp);//.united(pp);//.simplified();//
        //clrText.setAlpha(clrText.alpha());
        //p.fillPath(path, clrText);

        QPen penText(clrText);
        p.setPen(penText);
        p.drawText(nTextPosX,nTextPosY,strText);
    }
    imgSrc.save(sDesPngFileName);

    return bRet;
}

QString MyView::MD5String(const QString str)
{
    QString md5;
    QByteArray ba,bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(str);
    md.addData(ba);
    bb = md.result();
    md5.append(bb.toHex());
    return md5;
}

QString MyView::getOriginPngName(int type, bool isSel)
{
    QString path = QDir::currentPath() + PATH_ORIGIN_PRE;
    if(type == 1){
        path += PATH_COMMANDER;
    }else {
        path += PATH_SOLDIER;
    }
    if(isSel){
        path += PATH_SELECT;
    }
    path += ICON_TYPE;
    return path;
}

QString MyView::getDestPngName(int type, bool isSel, QString shortName, QColor color)
{
    QString path = QDir::currentPath() + PATH_DEST_PRE;
    if(type == 1){
        path += PATH_COMMANDER;
    }else {
        path += PATH_SOLDIER;
    }
    path += MD5String(shortName);
    path += color.name().mid(1);
    if(isSel)path += PATH_SELECT;
    path +=ICON_TYPE;
    return path;
}

void MyView::setReady()
{
    m_isLoadFinished = true;
}


