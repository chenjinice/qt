extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavdevice/avdevice.h"
}

#include "videothread.h"
#include "readthread.h"
#include <QDebug>
#include <QImage>

VideoThread::VideoThread(ReadThread *pthread)
{
    m_error = false;
    m_readThread = pthread;
    this->start();
}

VideoThread::~VideoThread()
{
    this->stopNow();
}

void VideoThread::addPacket(AVPacket *pkt)
{
    if(m_error){
        this->freeList();
        return;
    }
    AVPacket packet;
    memcpy(&packet,pkt,sizeof(AVPacket));
    m_list.append(packet);
}

int VideoThread::packetNum()
{
    return m_list.length();
}

void VideoThread::clear()
{
    m_list.clear();
}

void VideoThread::stopNow()
{
    m_running = false;
    this->quit();
    this->wait();
}

void VideoThread::waitStop()
{
    m_video_end = true;
    this->quit();
    this->wait();
}

void VideoThread::start(QThread::Priority pty)
{
    m_running = true;
    m_video_end = false;
    m_error = false;
    QThread::start(pty);
}

void VideoThread::run()
{
    if(!m_readThread) return;
    int videoStream = m_readThread->m_videoStream;
    AVCodecContext *pCodecCtx = m_readThread->m_formatCtx->streams[videoStream]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL){
        qDebug() << "can not find video decoder";
        m_error = true;
        return;
    }
    if(avcodec_open2(pCodecCtx,pCodec,NULL) < 0 ){
        qDebug() << "can not open video decoder";
        m_error = true;
        return;
    }
    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pFrameRGB = av_frame_alloc();
//    qDebug() << videoStream << ":" << pCodecCtx->width << "x" << pCodecCtx->height << "---pix_fmt:" << pCodecCtx->pix_fmt;
    AVPixelFormat pix_fmt = AV_PIX_FMT_RGB24;
    struct SwsContext *img_convert_ctx = NULL;
    int numBytes = -1;
    uint8_t *out_buffer = NULL;

    while (m_running) {
//        qDebug() << m_list.length() << ":" << m_readThread->m_path;
        if(m_list.length() == 0){
            if(m_video_end){
                break;
            }else {
                av_usleep(10000);
                continue;
            }
        }
        double spaceTime = (av_gettime()*1.0-m_readThread->m_startTime*1.0)/1000000;
        while (spaceTime < (m_list[0].dts-m_readThread->m_firstPts)*m_readThread->m_timebase){
            av_usleep(10000);
            spaceTime = (av_gettime()*1.0-m_readThread->m_startTime*1.0)/1000000;
        }

        int ret,got_pictrue;
        ret = avcodec_decode_video2(pCodecCtx,pFrame,&got_pictrue,&m_list[0]);
        if(ret < 0){
            qDebug() << "decode video error" << ",dts:" << m_list[0].dts;
            av_packet_unref(&m_list[0]);
            m_list.removeAt(0);
            continue;
        }

        if(got_pictrue){
//            if(img_convert_ctx == NULL)
//               img_convert_ctx = sws_getContext(pFrame->width,pFrame->height,(AVPixelFormat)pFrame->format,pFrame->width,
//                               pFrame->height,pix_fmt,SWS_BICUBIC,NULL,NULL,NULL);
//            img_convert_ctx = sws_getContext(pCodecCtx->width,pCodecCtx->height,pCodecCtx->pix_fmt,pCodecCtx->width,
//                                                                           pCodecCtx->height,pix_fmt,SWS_BICUBIC,NULL,NULL,NULL);
//            qDebug() << pFrame->width << "x" << pFrame->height << ":"<< pFrame->format;
            img_convert_ctx = sws_getCachedContext(img_convert_ctx,pFrame->width,pFrame->height,(AVPixelFormat)pFrame->format,pFrame->width,
                           pFrame->height,pix_fmt,SWS_BICUBIC,NULL,NULL,NULL);
            if(!img_convert_ctx){
                av_packet_unref(&m_list[0]);
                m_list.removeAt(0);
                continue;
            }
            if(numBytes == -1)numBytes = avpicture_get_size(pix_fmt,pFrame->width,pFrame->height);
            if(!out_buffer){
                out_buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
                avpicture_fill((AVPicture *)pFrameRGB,out_buffer,pix_fmt,pFrame->width,pFrame->height);
            }
            int ret = sws_scale(img_convert_ctx,(uint8_t const* const *)pFrame->data,pFrame->linesize,0,
                      pFrame->height,pFrameRGB->data,pFrameRGB->linesize);
            if(ret > 0){
                QImage image(out_buffer,pFrame->width,pFrame->height,QImage::Format_RGB888);
                emit m_readThread->getFrame(image.copy());
            }
        }
        av_packet_unref(&m_list[0]);
        m_list.removeAt(0);
    }
    if(out_buffer)av_free(out_buffer);
    if(pFrame) av_free(pFrame);
    if(pFrameRGB) av_free(pFrameRGB);
    if(pCodecCtx) avcodec_close(pCodecCtx);
    if(img_convert_ctx)sws_freeContext(img_convert_ctx);
    out_buffer = NULL;
    pFrame = NULL;
    pFrameRGB = NULL;
    pCodecCtx = NULL;
    img_convert_ctx = NULL;
    this->freeList();
}

void VideoThread::freeList()
{
    int length = m_list.length();
    for(int i=0;i<length;i++){
        av_packet_unref(&m_list[i]);
    }
    m_list.clear();
}



