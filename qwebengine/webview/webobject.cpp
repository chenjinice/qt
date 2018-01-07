#include "webobject.h"
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

WebObject::WebObject(QObject *parent) : QObject(parent)
{

}

WebObject::~WebObject()
{

}

void WebObject::sendStringToHtml(const QString &text)
{
    emit sendStringSignal(text);
}

void WebObject::addPosition(const QJsonArray &text)
{
    emit addPositionSignal(text);
}

void WebObject::updatePosition(const QJsonObject &text)
{
    emit updatePositionSignal(text);
}

void WebObject::selectSoldier(QString &soldierId,bool isCenter, int zoomLevel ,bool isJump)
{
    QJsonObject json;
    if(soldierId.isEmpty())return;
    json.insert("f_soldierid",soldierId);
    json.insert("isCenter",isCenter);
    json.insert("zoomLevel",zoomLevel);
    json.insert("isJump",isJump);
    emit selectSoldierSignal(json);
}

void WebObject::deleteSoldiers(const QJsonArray &text)
{
    emit deleteSoldiersSignal(text);
}

void WebObject::deleteAllSoldiers()
{
    emit deleteAllSoldiersSignal();
}

void WebObject::showPath(const QJsonObject &text)
{
    emit showPathSignal(text);
}

void WebObject::deleteAllPath()
{
    emit deleteAllPathSignal();
}

void WebObject::zoomToSeeAll()
{
    emit zoomToSeeAllSignal();
}

void WebObject::getVisible()
{
    emit getVisibleSignal();
}

void WebObject::prePosition()
{
    emit prePositionSignal();
}

void WebObject::recvSoldierIdSlot(const QString &text)
{
    emit clicked_soldierId(text);
}

void WebObject::recvVisibleIDSlot(const QString &text)
{
    QStringList list = text.split(",");
    emit recvVisibleID(list);
}



