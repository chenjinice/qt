#ifndef WEBOBJECT_H
#define WEBOBJECT_H

class QJsonArray;
class QJsonObject;

#include <QObject>

class WebObject : public QObject
{
    Q_OBJECT
public:
    explicit WebObject(QObject *parent = nullptr);
    ~WebObject();

    void sendStringToHtml(const QString &text);
    void addPosition(const QJsonArray &text);

    /*更新士兵在地图上的位置 : 数据为QJsonObject,包含的键名有:
     * type : QString类型 0:代表只更新下面soldiers数组中包含的士兵位置
     *                   1:代表除了更新数组中包含士兵位置，还要删除地图中存在但此数组中不存在的士兵
     * soldiers: QJsonArray，也就是json数组，数组元素为QJsonObject ，
      每个QJsonObject 包含的键名有:
      f_soldierid : 士兵id，QString类型
      f_soldiertype : 士兵类型，QString类型
      f_soldiername : 士兵名字，QString类型
      f_curlongitude : 经度，QString类型
      f_curlatitude : 纬度，QString类型
      f_unitinfo : info，QString类型
      icon       : 在地图上显示的图标，QString类型 */
    void updatePosition(const QJsonObject &text);

    // 选择地图中士兵，设置地图缩放级别的函数 ,soldierid : 士兵id ，isCenter : 是否居中, zoomLevel : 地图缩放级别
    // isJump : 是否出现跳动标标记
    void selectSoldier(QString &soldierId, bool isCenter, int zoomLevel , bool isJump = true);

    /*删除在地图上显示的士兵 : 数据为QJsonArray ，数组元素为QJsonObject
     * 每个QJsonObject 包含的键名有:
     * f_soldierid : 士兵id，QString类型
     */
    void deleteSoldiers(const QJsonArray &text);

    // 删除地图上的所有士兵
    void deleteAllSoldiers();

    /*显示路线，QJsonObject 包含的键名有:
     * f_soldierid : 士兵id，QString类型
     * color : 线的颜色，QString类型，如： "rgb(255,0,0)"
     * path  : 点的数组 ，QJsonArray类型，数组的元素为显示路线，QJsonObject
     *                                  包含的键名有：
     *                                             f_curlongitude : 经度，QString类型
     *                                             f_curlatitude : 纬度，QString类型
     */
    void showPath(const QJsonObject &text);

    //删除所有路线
    void deleteAllPath();

    // 缩放到显示所有士兵
    void zoomToSeeAll();

    /*此函数会发送 获取地图视野内所有士兵id 的信号
     *需要另外订阅此类的 recvVisibleID信号，以获取所有士兵id
     */
    void getVisible();

    //返回上一个位置
    void prePosition();

signals:
    void sendStringSignal(const QString &text);
    void addPositionSignal(const QJsonArray &text);
    void updatePositionSignal(const QJsonObject &text);
    void selectSoldierSignal(const QJsonObject &text);
    void deleteSoldiersSignal(const QJsonArray &text);
    void deleteAllSoldiersSignal();
    void showPathSignal(const QJsonObject &text);
    void deleteAllPathSignal();
    void zoomToSeeAllSignal();
    void getVisibleSignal();
    void prePositionSignal();

    void clicked_soldierId(const QString &soldierId);
    void recvVisibleID(const QStringList &list);

public slots:
    void recvSoldierIdSlot(const QString &text);
    void recvVisibleIDSlot(const QString &text);

private:

};

#endif // WEBOBJECT_H



