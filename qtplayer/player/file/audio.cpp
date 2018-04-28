extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavdevice/avdevice.h"
}

#include "audio.h"
#include "decoderfile.h"
#include "../plugin/clog.h"
#include <QThread>
#include <QDebug>

Audio::Audio(DecoderFile *decoder)
{
    m_isRunning = false;
    m_isEnd = true;
    m_decoder = decoder;
    m_thread = new QThread;
    this->moveToThread(m_thread);
    connect(m_thread,&QThread::started,this,&Audio::startThread);
}

Audio::~Audio()
{
    if(m_thread)delete m_thread;
}

void Audio::addPacket(AVPacket *pkt)
{
    m_mutex.lock();
    m_list.append(*pkt);
    m_mutex.unlock();
}

void Audio::startRun()
{
    if(m_isRunning)return;
    m_isRunning = true;
    m_isEnd = false;
    m_thread->start();
}

int Audio::packetNum()
{
    return m_list.length();
}

void Audio::stopNow()
{
    m_isRunning = false;
    m_thread->quit();
    m_thread->wait();
    this->freeList();
}

void Audio::waitEnd()
{
    m_isEnd = true;
    m_thread->quit();
    m_thread->wait();
    this->freeList();
    m_isRunning = false;
}

void Audio::clearList()
{
    this->freeList();
}

void Audio::startThread()
{
    while (m_isRunning) {
        if(m_decoder->m_isPause){
            av_usleep(100000);
            continue;
        }
        AVPacket packet1,*packet = &packet1;
        av_init_packet(packet);
        if(getOnePacket(packet) < 0){
            if(m_isEnd)break;
            av_usleep(10000);
            continue;
        }

        int64_t pts = m_decoder->getPacketPts(packet);
        double playedTime = (av_gettime()-m_decoder->m_count_BaseTime)*1.0/1000000.0;
        double shouldPlayTme = (pts-m_decoder->m_count_BasePts)*1.0*m_decoder->m_stream_timebase;
        while ((playedTime < shouldPlayTme) && m_isRunning){
//            qDebug() << playedTime << " :::" << shouldPlayTme;
            av_usleep(10000);
            playedTime = (av_gettime()-m_decoder->m_count_BaseTime)*1.0/1000000.0;
        }

        int ret,got_audio;
        AVFrame *pFrame = m_decoder->m_frame_audio;
        ret = avcodec_decode_audio4(m_decoder->m_CodecCtx_audio,pFrame,&got_audio,packet);
        if(ret < 0){
            c_log("decode_audio4 error: ret=%d,.pts=%ld,.dts=%ld,pts=%ld,size=%d,buf=%p,data=%p,side_data=%p",
                  ret,packet->pts,packet->dts,pts,packet->size,packet->buf,packet->data,packet->side_data);
            m_decoder->freePacket(packet);
            continue;
        }
        if(got_audio){
            int ret = swr_convert(m_decoder->m_swrCtr,&m_decoder->m_buffer_audio,CAUDIO_BUFFER_SIZE,(const uint8_t **)pFrame->data,pFrame->nb_samples);
            if(ret > 0){
                m_decoder->writeAudioData();
            }
            m_decoder->currentUITime(pts);
        }else {
            c_log("decode_audio4 got_audio error: got_audio=%d,.pts=%ld,.dts=%ld,pts=%ld,size=%d,buf=%p,data=%p,side_data=%p",
                  got_audio,packet->pts,packet->dts,pts,packet->size,packet->buf,packet->data,packet->side_data);
        }
        m_decoder->freePacket(packet);
    }
}

int Audio::getOnePacket(AVPacket *pkt)
{
    m_mutex.lock();
    int ret = -1;
    int length = m_list.length();
    if(length > 0){
       *pkt = m_list[0];
        m_list.removeAt(0);
        ret = 0;
    }else {
        ret = -1;
    }
    m_mutex.unlock();
    return ret;
}

void Audio::freeList()
{
    m_mutex.lock();
    int length = m_list.length();
    for(int i=0;i<length;i++){
        m_decoder->freePacket(&m_list[i]);
    }
    m_list.clear();
    m_mutex.unlock();
}

