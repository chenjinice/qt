#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <zmq.h>
#include <pthread.h>

#include "server.h"
#include "fusion.pb.h"
#include "vision.pb.h"
#include "radar.pb.h"


using namespace vision;
using namespace fusionPackage;
using namespace radarPackage;


#define BUFFER_SIZE 10240


Server::Server(uint16_t port, char *filter)
{
    int ret;
    m_port = port;
    strcpy(m_filter,filter);

    char pub_endpoint[50] = {0};
    sprintf(pub_endpoint,"tcp://*:%d",m_port);
    printf("server : zmq pub = %s , filter = %s\n",pub_endpoint,m_filter);
    m_context = zmq_ctx_new();
    m_publisher = zmq_socket(m_context,ZMQ_PUB);
    ret = zmq_bind(m_publisher,pub_endpoint);
    if(ret != 0){
        perror("server : zmq bind error");
        exit(-1);
    }
}

Server::~Server()
{
    zmq_close(m_publisher);
    zmq_ctx_destroy(m_context);
}

void Server::send()
{
    int len;
    uint8_t buffer[BUFFER_SIZE] = {0};

    if(strcmp(m_filter,"vision")==0)len = set_vision(buffer);
    else if(strcmp(m_filter,"fusion")==0)len = set_fusion(buffer);
    else if(strcmp(m_filter,"radar")==0)len = set_radar(buffer);
    else return;

    if(len > BUFFER_SIZE){
        printf("server : pack size = %d > %d(MAX)\n",len,BUFFER_SIZE);
        return;
    }
    zmq_send(m_publisher,m_filter,strlen(m_filter),ZMQ_SNDMORE);
    zmq_send(m_publisher,buffer,len,0);
}


int Server::set_vision(uint8_t *buffer)
{
    int len;
    static int s_lng = 1128668350;
    static int s_lat = 281128874;
    static int s_count = 0;
    s_count++;

    Crowd a;
    Pedestrian *p;
    a.set_id(TRAFFIC_STATUS);
    for(int i=0;i<3;i++){
        int offset = i*1000 + (s_count%50)*300;
        p = a.add_pedestrian();
        p->set_object_id(i);
        p->set_object_type(HUMAN);      // 类型:人,车,摩托车
        p->set_camera(1);               // 哪个相机 :1,2,3,4
        p->set_lng(s_lng+offset);         // 经度
        p->set_lat(s_lat+offset);	        // 纬度
    }
    len = a.ByteSize();
    a.SerializeToArray(buffer,len);
    return len;
}


int Server::set_fusion(uint8_t *buffer)
{
    int len;
    static int s_lng = 1128668350;
    static int s_lat = 281128874;
    static int s_count = 0;
    s_count++;

    fusionMsg m;
    fusionPackage::targetMessage *p;
    for(int i=0;i<2;i++){
        int offset = i*1000 + (s_count%50)*300;
        p = m.add_msg();
        p->set_id(i+100);
        p->set_lon(s_lng+offset);
        p->set_lat(s_lat-offset);
        p->set_heading(100);
        p->set_type(1);
        p->set_velocity(1000);
    }
    fusionPackage::Timestamp *t = new fusionPackage::Timestamp();
    t->set_nanos(111);
    t->set_seconds(2222);
    m.set_allocated_timestamp(t);

    len = m.ByteSize();
    m.SerializeToArray(buffer,len);
    return len;
}


int Server::set_radar(uint8_t *buffer)
{
    int len;
    static int s_lng = 1128668350;
    static int s_lat = 281128874;
    static int s_count = 0;
    s_count++;

    radarMsg m;
    radarPackage::targetMessage *p;
    for(int i=0;i<4;i++){
        int offset = i*1000 + (s_count%50)*300;
        p = m.add_msg();
        p->set_id(i+2000);
        p->set_longitudinalx(s_lat+offset);
        p->set_lateraly(s_lng-offset);
        p->set_heading(100);
        p->set_rcs(111);
        p->set_velocity(111);
    }
    radarPackage::Timestamp *t = new radarPackage::Timestamp();
    t->set_nanos(111111);
    t->set_seconds(222222);
    m.set_allocated_time(t);

    len = m.ByteSize();
    m.SerializeToArray(buffer,len);
    return len;
}
