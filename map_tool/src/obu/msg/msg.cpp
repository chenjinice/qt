/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */


#include "msg.h"

#include <unistd.h>
#include <string.h>
#include <thread>
#include <mutex>
#include "device/this_device.h"

#include <QDir>
#include <QDateTime>
#include <QFile>


void getMapJson(LocalMap &map,QJsonObject &json){
    int count = map.nodes.size();
    QJsonArray node_array;

    for (int i=0;i<count;i++){
        const LocalNode &node = map.nodes[i];
        QJsonObject node_obj;
        QJsonArray  lane_array;
        node_obj.insert("id",node.id.id);
        node_obj.insert("region",node.id.region);
        node_obj.insert("lng",node.pos.lng);
        node_obj.insert("lat",node.pos.lat);

        int lane_count = node.lanes.size();
        for(int m=0;m<lane_count;m++){
            const LocalLane &lane = node.lanes[m];
            QJsonObject lane_obj;
            QJsonArray  pt_array;
            lane_obj.insert("lane_id",lane.lane_id);
            lane_obj.insert("lane_width",lane.lane_width);
            lane_obj.insert("maneuvers",lane.dir);
            lane_obj.insert("phase_left",lane.phases.left);
            lane_obj.insert("phase_straight",lane.phases.straight);
            lane_obj.insert("phase_right",lane.phases.right);

            int pt_count = lane.points.size();
            for(int n=0;n<pt_count;n++){
                const PosWGS84 &pt = lane.points[n];
                QJsonObject pt_obj;
                pt_obj.insert("lng",pt.lng);
                pt_obj.insert("lat",pt.lat);
                pt_array.append(pt_obj);
            }
            lane_obj.insert("points",QJsonArray(pt_array));
            lane_array.append(lane_obj);
        }
        node_obj.insert("lanes",QJsonValue(lane_array));
        node_array.append(node_obj);
    }
    json.insert("nodes",QJsonValue(node_array));
    json.insert("type","map_asn");
}

void getRsiJson(LocalRsi &rsi,QJsonObject &json){
    int rtes_count = rsi.rtes.size();
    int rtss_count = rsi.rtss.size();
    QJsonArray rtes_array;
    QJsonArray rtss_array;
    // rtes
    for (int i=0;i<rtes_count;i++){
        const LocalRte &rte = rsi.rtes[i];
        QJsonObject rte_obj;
        QJsonArray  path_array;
        rte_obj.insert("eventType",rte.type);
        rte_obj.insert("lng",rte.event_pos.lng);
        rte_obj.insert("lat",rte.event_pos.lat);
        rte_obj.insert("description",rte.des.c_str());
        int path_count = rte.paths.size();
        for(int m=0;m<path_count;m++){
            const LocalRefPath &path = rte.paths[m];
            QJsonObject path_obj;
            QJsonArray  pt_array;
            path_obj.insert("pathRadius",path.radius);
            int pt_count = path.points.size();
            for(int n=0;n<pt_count;n++){
                const PosWGS84 &pt = path.points[n];
                QJsonObject pt_obj;
                pt_obj.insert("lng",pt.lng);
                pt_obj.insert("lat",pt.lat);
                pt_array.append(pt_obj);
            }
            path_obj.insert("activePath",QJsonArray(pt_array));
            path_array.append(path_obj);
        }
        rte_obj.insert("referencePaths",QJsonValue(path_array));
        rtes_array.append(rte_obj);
    }
    // rtss
    for (int i=0;i<rtss_count;i++){
        const LocalRts &rts = rsi.rtss[i];
        QJsonObject rts_obj;
        QJsonArray  path_array;
        rts_obj.insert("signType",rts.type);
        rts_obj.insert("lng",rts.sign_pos.lng);
        rts_obj.insert("lat",rts.sign_pos.lat);
        rts_obj.insert("description",rts.des.c_str());
        int path_count = rts.paths.size();
        for(int m=0;m<path_count;m++){
            const LocalRefPath &path = rts.paths[m];
            QJsonObject path_obj;
            QJsonArray  pt_array;
            path_obj.insert("pathRadius",path.radius);
            int pt_count = path.points.size();
            for(int n=0;n<pt_count;n++){
                const PosWGS84 &pt = path.points[n];
                QJsonObject pt_obj;
                pt_obj.insert("lng",pt.lng);
                pt_obj.insert("lat",pt.lat);
                pt_array.append(pt_obj);
            }
            path_obj.insert("activePath",QJsonArray(pt_array));
            path_array.append(path_obj);
        }
        rts_obj.insert("referencePaths",QJsonValue(path_array));
        rtss_array.append(rts_obj);
    }
    json.insert("rtes",QJsonValue(rtes_array));
    json.insert("rtss",QJsonValue(rtss_array));
    json.insert("type","rsi_asn");
//    qDebug() << json;
}

// --------------------------------------------------------------------------------

static QString getLogDirect(QString ip){
    return  "./asn_log/" + ip + "/" + QDateTime::currentDateTime().toString("yyyyMMdd");
}

static void saveFun(uint8_t *buffer,int len,QString direct,QString path) {
    QDir dir;
    QFile file(path);
    if(file.exists())return;
    if(!dir.exists(direct))dir.mkpath(direct);
    bool ret = file.open(QIODevice::ReadWrite);
    if(!ret)return;
    file.write((const char *)(buffer),len);
    file.close();
}

static void saveMap(uint8_t *buffer,int len,const LocalMap &map,QString ip)
{
    if(map.nodes.size() == 0)return;
    QString direct = getLogDirect(ip);
    QString path   = QString("%1/map_%2_%3.%4.per").arg(direct).arg(map.nodes[0].id.id).arg(map.nodes[0].id.region)
                     .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmm"));
    saveFun(buffer,len,direct,path);
}

static void saveRsi(uint8_t *buffer,int len,const LocalRsi &rsi,QString ip)
{
    int rtes_count = rsi.rtes.size();
    int rtss_count = rsi.rtss.size();

    QString direct = getLogDirect(ip);
    QString path;
    if(rtss_count > 0){
        path   = QString("%1/rsi_rts_%2.%3.per").arg(direct).arg(rsi.rtss[0].type)
                .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmm"));
        saveFun(buffer,len,direct,path);
    }else if(rtes_count > 0) {
        path   = QString("%1/rsi_rte_%2.%3.per").arg(direct).arg(rsi.rtes[0].type)
                .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmm"));
        saveFun(buffer,len,direct,path);
    }
}

// --------------------------------------------------------------------------------

// 解析buffer ，并写场景代码
void msgDecode(uint8_t *buffer, int len, QString ip, const timeval &tv, QJsonObject &json, bool flag)
{
    LocalBsm   bsm;
    LocalMap   map;
    LocalRsm   rsm;
    LocalRsi   rsi;
    LocalSpat  spat;

    asn_dec_rval_t   ret;
    MessageFrame_t * msg            = nullptr;
    ret = uper_decode_complete(nullptr,&asn_DEF_MessageFrame,(void **)&msg,buffer,len);
    if(ret.code != RC_OK){
        qDebug("uper_decode_complete failed\n");
        goto GO_END;
    }
    switch (msg->present) {
        case MessageFrame_PR_bsmFrame:
//            bsmToLocal(msg->choice.bsmFrame,bsm,tv);
            bsmFunc(bsm);
            break;
        case MessageFrame_PR_mapFrame:
            mapToLocal(msg->choice.mapFrame,map,tv);
            MapCache::instance()->add(map);
            if(flag)saveMap(buffer,len,map,ip);
            getMapJson(map,json);
            break;
        case MessageFrame_PR_rsmFrame:
            rsmToLocal(msg->choice.rsmFrame,rsm,tv);
            rsmFunc(rsm);
            break;
        case MessageFrame_PR_spatFrame:
            spatToLocal(msg->choice.spatFrame,spat,tv);
            spatFunc(spat);
            break;
        case MessageFrame_PR_rsiFrame:
            rsiToLocal(msg->choice.rsiFrame,rsi,tv);
            rsiFunc(rsi);
            if(flag)saveRsi(buffer,len,rsi,ip);
            getRsiJson(rsi,json);
            break;
        case MessageFrame_PR_NOTHING:
        default:
            break;
    }
GO_END:
    if(msg){
        ASN_STRUCT_FREE(asn_DEF_MessageFrame,msg);
    }
}


