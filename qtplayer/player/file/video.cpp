extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavdevice/avdevice.h"
}

#include "video.h"
#include "decoderfile.h"
#include "../plugin/clog.h"
#include <QThread>
#include <QDebug>

#define  PIX_FMT  AV_PIX_FMT_RGB24

Video::Video(DecoderFile *decoder)
{
    m_isRunning = false;
    m_isEnd = true;
    m_decoder = decoder;
    m_thread = new QThread;
    this->moveToThread(m_thread);
    connect(m_thread,&QThread::started,this,&Video::startThread);

}

Video::~Video()
{
    if(m_thread)delete m_thread;
}

void Video::addPacket(AVPacket *pkt)
{
    m_mutex.lock();
    m_list.append(*pkt);
    m_mutex.unlock();
}

void Video::startRun()
{
    if(m_isRunning)return;
    m_isRunning = true;
    m_isEnd = false;
    m_thread->start();
}

int Video::packetNum()
{
    return m_list.length();
}

void Video::stopNow()
{
    m_isRunning = false;
    m_thread->quit();
    m_thread->wait();
    this->freeList();
}

void Video::waitEnd()
{
    m_isEnd = true;
    m_thread->quit();
    m_thread->wait();
    this->freeList();
    m_isRunning = false;
}

void Video::clearList()
{
    this->freeList();
}

void Video::startThread()
{
    struct SwsContext *img_convert_ctx = NULL;
    int numBytes = -1;
    uint8_t *out_buffer = NULL;
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
        double shouldPlayTme = (pts-m_decoder->m_count_BasePts)*m_decoder->m_stream_timebase;
        while (playedTime < shouldPlayTme && m_isRunning){
            av_usleep(10000);
            playedTime = (av_gettime()-m_decoder->m_count_BaseTime)*1.0/1000000.0;
        }

        int ret,got_pictrue;
        AVFrame *pFrame = m_decoder->m_frame_video;
        AVFrame *pFrameRGB = m_decoder->m_frameRGB;
        ret = avcodec_decode_video2(m_decoder->m_CodecCtx_video,pFrame,&got_pictrue,packet);
        if(ret < 0){
            c_log("decode_video2 error: ret=%d,.pts=%ld,.dts=%ld,pts=%ld,size=%d,buf=%p,data=%p,side_data=%p",
                  ret,packet->pts,packet->dts,pts,packet->size,packet->buf,packet->data,packet->side_data);
            m_decoder->freePacket(packet);
            continue;
        }

        if(got_pictrue){
            img_convert_ctx = sws_getCachedContext(img_convert_ctx,pFrame->width,pFrame->height,(AVPixelFormat)pFrame->format,pFrame->width,
                           pFrame->height,PIX_FMT,SWS_BICUBIC,NULL,NULL,NULL);
            if(!img_convert_ctx){
                c_log("video sws_getCachedContext error: ret=%d,.pts=%ld,.dts=%ld,pts=%ld,size=%d,buf=%p,data=%p,side_data=%p",
                      ret,packet->pts,packet->dts,pts,packet->size,packet->buf,packet->data,packet->side_data);
                m_decoder->freePacket(packet);
                continue;
            }
            if(numBytes == -1)numBytes = avpicture_get_size(PIX_FMT,pFrame->width,pFrame->height);
            if(!out_buffer){
                out_buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
                avpicture_fill((AVPicture *)pFrameRGB,out_buffer,PIX_FMT,pFrame->width,pFrame->height);
            }
            int ret = sws_scale(img_convert_ctx,(uint8_t const* const *)pFrame->data,pFrame->linesize,0,
                      pFrame->height,pFrameRGB->data,pFrameRGB->linesize);
            if(ret > 0){
                QImage image(out_buffer,pFrame->width,pFrame->height,QImage::Format_RGB888);
                emit m_decoder->getFrame(image.copy());
            }
            m_decoder->currentUITime(pts);
        }else{
            c_log("video got_picture error: ret=%d,.pts=%ld,.dts=%ld,pts=%ld,size=%d,buf=%p,data=%p,side_data=%p",
                  ret,packet->pts,packet->dts,pts,packet->size,packet->buf,packet->data,packet->side_data);
        }
        m_decoder->freePacket(packet);
    }
    if(out_buffer)av_free(out_buffer);
    if(img_convert_ctx)sws_freeContext(img_convert_ctx);
}

int Video::getOnePacket(AVPacket *pkt)
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

void Video::freeList()
{
    m_mutex.lock();
    int length = m_list.length();
    for(int i=0;i<length;i++){
        m_decoder->freePacket(&m_list[i]);
    }
    m_list.clear();
    m_mutex.unlock();
}





