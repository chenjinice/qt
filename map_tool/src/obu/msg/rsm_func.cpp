/*
 * Copyright 2021 Cidi.chen. All Rights Reserved.
 */


#include "msg.h"

#include <unistd.h>

#include "device/this_device.h"

#include <QDebug>

// rsm场景 
void rsmFunc(const LocalRsm &rsm)
{
//    rsm.show();
    ThisDevice *me = ThisDevice::instance();
    SceneConfig &cfg = me->scenes;

    int count = rsm.ptcs.size();
    for(int i=0;i<count;i++){
        const LocalParticipant &ptc = rsm.ptcs[i];
        qDebug("rsm[%d/%d] : type=%d,ptcId=%d,lng=%.07lf,lat=%.07lf",i+1,count,ptc.ptc_type,ptc.ptc_id,ptc.pos.lng,ptc.pos.lat);
        if(cfg.vrucw && (ptc.ptc_type == ParticipantType_pedestrian) ) appVrucw(*me,ptc.pos);
    }
}
