extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavdevice/avdevice.h"
}
#include "readthread.h"
#include "videothread.h"
#include "audiothread.h"
#include <QDebug>

int ReadThread::m_count = 0;

ReadThread::ReadThread()
{
    m_reading = false;
//    if(m_count++ <= 0 ){
    av_register_all();
    avformat_network_init();
//    }
    m_startTime = -1;
    m_firstPts = -1;
    m_timebase = -1;
    m_formatCtx = NULL;
    m_videoThread = NULL;
    m_audioThread = NULL;
}

ReadThread::~ReadThread()
{
    this->stop();
    avformat_network_deinit();
}

void ReadThread::setFilePath(const QString path)
{
    m_path = path;
}

void ReadThread::start(QThread::Priority pty)
{
    m_startTime = -1;
    m_firstPts = -1;
    m_timebase = -1;
    m_reading = true;
    QThread::start(pty);
}

void ReadThread::stop()
{
    m_reading = false;
    this->quit();
    this->wait();
}

void ReadThread::run()
{
    std::string t_str = m_path.toStdString();
    const char *file_path = t_str.c_str();

    if(!m_formatCtx)m_formatCtx = avformat_alloc_context();
    AVFormatContext *a = m_formatCtx;
    if(avformat_open_input(&m_formatCtx,file_path,NULL,NULL) !=0){
        qDebug() << "can not open file :" << file_path;
        avformat_close_input(&m_formatCtx);
        return;
    }
    m_formatCtx->flags |= AVFMT_FLAG_NOBUFFER;
//    avformat_flush(m_formatCtx);
    qDebug() << "open stream      -------- ";
    if(avformat_find_stream_info(m_formatCtx,NULL) < 0){
        qDebug() << "can not find stream information";
        avformat_close_input(&m_formatCtx);
        return;
    }
//    qDebug() << "find stream info -------- ";
    //查找视频流和音频流
    m_videoStream = -1,m_audioStream = -1;
    for(int i=0;i<m_formatCtx->nb_streams;i++){
        if(m_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            m_videoStream = i;
        }
        if(m_formatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
            m_audioStream = i;
        }
    }
    if(m_videoStream == -1 && m_audioStream == -1){
        qDebug() << "can not find stream";
        return;
    }
    if(m_videoStream != -1){
        if(!m_videoThread)m_videoThread = new VideoThread(this);
    }
    if(m_audioStream != -1){
        if(!m_audioThread)m_audioThread = new AudioThread(this);
    }

    int t_index = -1;
    if(m_videoStream != -1)t_index = m_videoStream;
    else if (m_audioStream != -1)t_index = m_audioStream;
    m_timebase = m_formatCtx->streams[t_index]->time_base.num*1.0/m_formatCtx->streams[t_index]->time_base.den*1.0;
    m_startTime = av_gettime()+200000;
    AVPacket *packet = (AVPacket *)malloc(sizeof(AVPacket));
//    qDebug() << "m_reading = " << m_reading;
    while (m_reading) {
        int v_frame_num = m_videoThread ? m_videoThread->packetNum():-1;
        int a_frame_num = m_audioThread ? m_audioThread->packetNum():-1;
        if(v_frame_num > 50 || a_frame_num > 50){
            av_usleep(10000);
            continue;
        }
        qDebug() << "===== before read frame ===== ";
        if(av_read_frame(m_formatCtx,packet) < 0){
            break; // 认为数据已经读完了
        }
        qDebug() << packet->stream_index << ":" << packet->pts << "---" << packet->dts;
        if(m_firstPts == -1)m_firstPts = packet->dts;
        if(packet->stream_index == m_videoStream){
//            qDebug() << packet->stream_index << ":" << packet->pts  << "----" << packet->dts;
            m_videoThread->addPacket(packet);
        }else if(packet->stream_index == m_audioStream){
//            qDebug() << packet->stream_index << ":" << packet->pts  << "----" << packet->dts;
            m_audioThread->addPacket(packet);
        }else {
            av_packet_unref(packet);
        }
    }
    if(m_reading){
        if(m_videoThread)m_videoThread->waitStop();
        if(m_audioThread)m_audioThread->waitStop();
    }else {
        if(m_videoThread)m_videoThread->stopNow();
        if(m_audioThread)m_audioThread->stopNow();
    }
    if(m_videoThread){
        delete m_videoThread;
        m_videoThread = NULL;
    }
    if(m_audioThread){
        delete m_audioThread;
        m_audioThread = NULL;
    }
    free(packet);
    avformat_close_input(&m_formatCtx);
}
