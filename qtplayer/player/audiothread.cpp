extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavdevice/avdevice.h"
}

#include "audiothread.h"
#include "readthread.h"
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <QDebug>

#define AUDIO_BUFFER_SIZE 192000

AudioThread::AudioThread(ReadThread *pthread)
{
    m_audio_output = NULL;
    m_audio_IOdevice = NULL;
    m_error = false;
    m_readThread = pthread;
    this->start();
}

AudioThread::~AudioThread()
{
    this->stopNow();
}

void AudioThread::addPacket(AVPacket *pkt)
{
    if(m_error){
        this->freeList();
        return;
    }
    if(!this->isRunning())this->start();
    AVPacket packet;
    memcpy(&packet,pkt,sizeof(AVPacket));
    m_list.append(packet);
}

int AudioThread::packetNum()
{
    return m_list.length();
}

void AudioThread::clear()
{
    m_list.clear();
}

void AudioThread::stopNow()
{
    m_running = false;
    this->quit();
    this->wait();
}

void AudioThread::waitStop()
{
    m_audio_end = true;
    this->quit();
    this->wait();
}

void AudioThread::start(QThread::Priority pty)
{
    m_running = true;
    m_audio_end = false;
    m_error = false;
    QThread::start(pty);
}

void AudioThread::run()
{
    if(!m_readThread) return;
    int audioStream = m_readThread->m_audioStream;
    AVCodecContext *pCodecCtx = m_readThread->m_formatCtx->streams[audioStream]->codec;
    AVCodec *pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec == NULL){
        qDebug() << "can not find audio decoder";
        m_error = true;
        return;
    }
    if(avcodec_open2(pCodecCtx,pCodec,NULL) < 0 ){
        qDebug() << "can not open audio decoder";
        m_error = true;
        return;
    }

    AVFrame *pFrame = av_frame_alloc();
    SwrContext *swrCtr = swr_alloc();
    AVSampleFormat out_samput_fmt = AV_SAMPLE_FMT_S16;
    int in_channel_layout = av_get_default_channel_layout(pCodecCtx->channels);
    int out_channel_layout = in_channel_layout;
    swr_alloc_set_opts(swrCtr,out_channel_layout,out_samput_fmt,pCodecCtx->sample_rate,
                       in_channel_layout,pCodecCtx->sample_fmt,pCodecCtx->sample_rate,0,NULL);
    swr_init(swrCtr);
    int sample_size = av_samples_get_buffer_size(NULL,1,1,out_samput_fmt,1)*8;
    this->creatAudioOutput(pCodecCtx->sample_rate,pCodecCtx->channels,sample_size);
    uint8_t *out_buffer =(uint8_t *)av_malloc(AUDIO_BUFFER_SIZE);

    while (m_running) {
//        qDebug() << m_list.length() << ":" << m_readThread->m_path;
        if(m_list.length() == 0){
            if(m_audio_end){
                break;
            }else {
                usleep(10000);
                continue;
            }
        }
        double spaceTime = (av_gettime()*1.0-m_readThread->m_startTime*1.0)/1000000;
//        qDebug() << "space time  :  " << spaceTime << "-- timebase : " << m_readThread->m_timebase;
        while (spaceTime < (m_list[0].dts-m_readThread->m_firstPts)*m_readThread->m_timebase){
            usleep(10000);
            spaceTime = (av_gettime()*1.0-m_readThread->m_startTime*1.0)/1000000;
        }

        int ret,got_audio;
        ret = avcodec_decode_audio4(pCodecCtx,pFrame,&got_audio,&m_list[0]);
        if(ret < 0){
            qDebug() << "decode audio error";
            av_packet_unref(&m_list[0]);
            m_list.removeAt(0);
            continue;
        }
        if(got_audio){
            int ret = swr_convert(swrCtr,&out_buffer,AUDIO_BUFFER_SIZE,(const uint8_t **)pFrame->data,pFrame->nb_samples);
            int buff_size = av_samples_get_buffer_size(NULL,pCodecCtx->channels,pFrame->nb_samples,out_samput_fmt,1);
            if(ret > 0)m_audio_IOdevice->write((char *)out_buffer,buff_size);
        }
        av_packet_unref(&m_list[0]);
        m_list.removeAt(0);
    }
    av_free(pFrame);
    av_free(out_buffer);
    swr_free(&swrCtr);
    avcodec_close(pCodecCtx);
    this->freeList();
}

bool AudioThread::creatAudioOutput(int sample_rate, int channels, int sample_size)
{
    if(m_audio_output){
        m_audio_output->stop();
        delete m_audio_output;
        m_audio_output = NULL;
        m_audio_IOdevice = NULL;
    }
    QAudioFormat format;
    format.setSampleRate(sample_rate);
    format.setChannelCount(channels);
    format.setCodec("audio/pcm");
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleSize(sample_size);
    format.setByteOrder(QAudioFormat::LittleEndian);
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if(!info.isFormatSupported(format)){
        qDebug() << "audio format not supported";
        return false;
    }
    m_audio_output = new QAudioOutput(format);
    m_audio_IOdevice = m_audio_output->start();
    return true;
}

void AudioThread::freeList()
{
    int length = m_list.length();
    for(int i=0;i<length;i++){
        av_packet_unref(&m_list[i]);
    }
    m_list.clear();
}
