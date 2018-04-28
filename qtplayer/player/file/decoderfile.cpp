extern "C"{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
#include "libavdevice/avdevice.h"
}

#include "decoderfile.h"
#include "video.h"
#include "audio.h"
#include "../plugin/clog.h"
#include <QDebug>
#include <QThread>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioOutput>

//AV_SAMPLE_FMT_S32

DecoderFile::DecoderFile()
{
    this->initParam();
    m_video = new Video(this);
    m_audio = new Audio(this);
    m_thread_read = new QThread;

    this->moveToThread(m_thread_read);
    connect(m_thread_read,&QThread::started,this,&DecoderFile::startReadThread);
}

DecoderFile::~DecoderFile()
{
    if(m_thread_read)delete m_thread_read;
}

void DecoderFile::setUrl(const QString &path)
{
    m_file_path = path;
}

void DecoderFile::startPlay()
{
    if(m_isReading)return;
    m_thread_read->quit();
    m_thread_read->wait();
    m_isReading = true;
    m_isPause = false;
    m_stream_timebase = 0.01;
    m_firstPts = AV_NOPTS_VALUE;
    m_count_BasePts = AV_NOPTS_VALUE;
    m_thread_read->start();
}

void DecoderFile::stop()
{
    m_isReading = false;
    m_thread_read->quit();
    m_thread_read->wait();
}

void DecoderFile::pause()
{
    if(!m_isReading)return;
    if(m_isPause){
        m_count_BasePts = AV_NOPTS_VALUE;
        m_count_BaseTime = 0;
        m_isPause = false;
//        m_count_BaseTime = AV_NOPTS_VALUE;
    }else {
        m_isPause = true;
    }
}

void DecoderFile::setVolume(int volume)
{
    m_volume = volume/100.0;
    if(m_audio_output){
        m_audio_output->setVolume(m_volume);
    }
}

void DecoderFile::seek(int pos)
{
    if(!m_isReading)return;
    if(m_packet_readEnd)return;
    if(m_firstPts == AV_NOPTS_VALUE)return;
    int t_index = -1;
    if(m_videoStream != -1)t_index = m_videoStream;
    else if(m_audioStream != -1)t_index = m_audioStream;

    int64_t target_pos = (int64_t)(pos/m_stream_timebase)+m_firstPts;
    int64_t max_pos    = m_formatCtx->duration/AV_TIME_BASE/m_stream_timebase+m_firstPts;
//    qDebug() << target_pos << ":" << m_formatCtx->duration << ":" << m_firstPts << ":" << m_formatCtx->duration/AV_TIME_BASE/m_stream_timebase;
    if(target_pos >= max_pos || target_pos <= m_firstPts)return;

    m_mutex.lock();
    m_video->clearList();
    m_audio->clearList();
    av_usleep(200000);
    if(m_CodecCtx_audio)avcodec_flush_buffers(m_CodecCtx_audio);
    if(m_CodecCtx_video)avcodec_flush_buffers(m_CodecCtx_video);
    av_seek_frame(m_formatCtx,t_index,target_pos,AVSEEK_FLAG_BACKWARD);
    m_count_BasePts = AV_NOPTS_VALUE;
//    m_count_BaseTime = av_gettime();
    m_mutex.unlock();
}

bool DecoderFile::isReading()
{
    return m_isReading;
}

void DecoderFile::initParam()
{
    m_video = NULL;
    m_audio = NULL;
    m_thread_read = NULL;
    m_formatCtx = NULL;
    m_audioStream = -1;
    m_videoStream = -1;
    m_isPause   = false;
    m_isReading = false;
    m_packet_readEnd = false;
    m_isAudioReady = false;
    m_isVideoReady = false;
    m_CodecCtx_audio = NULL;
    m_CodecCtx_video = NULL;
    m_Codec_audio = NULL;
    m_Codec_video = NULL;
    m_frame_video = NULL;
    m_frameRGB    = NULL;
    m_frame_audio = NULL;
    m_swrCtr = NULL;
    m_buffer_audio = NULL;

    m_audio_IOdevice = NULL;
    m_audio_output = NULL;
    m_volume = 1.0;

    m_stream_timebase = 0.01;
    m_firstPts = AV_NOPTS_VALUE;
    m_count_BasePts = AV_NOPTS_VALUE;
    m_count_BaseTime = 0;

    av_register_all();
}

void DecoderFile::startReadThread()
{
    std::string t_str = m_file_path.toStdString();
    const char *file_path = t_str.c_str();

    if(!m_formatCtx)m_formatCtx = avformat_alloc_context();
    if(avformat_open_input(&m_formatCtx,file_path,NULL,NULL) !=0){
        c_log("can not open file : "+m_file_path);
        avformat_close_input(&m_formatCtx);
        m_isReading = false;
        return;
    }
//    qDebug() << "after open ===========";
    if(avformat_find_stream_info(m_formatCtx,NULL) < 0){
        c_log("can not find stream information");
        avformat_close_input(&m_formatCtx);
        m_isReading = false;
        return;
    }
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
    this->openAudioDecoder();
    this->openVideoDecoder();
    if(!m_isAudioReady && !m_isVideoReady){        //找解码器
        avformat_close_input(&m_formatCtx);
        m_isReading = false;
        return;
    }
    getStreamTimeBase();
    if(m_formatCtx->duration != AV_NOPTS_VALUE){
        emit this->videoLength(m_formatCtx->duration/1000000);
    }
    AVPacket packet1, *packet = &packet1;
    av_init_packet(packet);
//    m_count_BaseTime = av_gettime();
    while (m_isReading) {
        if(m_isPause){
            av_usleep(100000);
            continue;
        }
        int vNum = m_video->packetNum();
        int aNum = m_audio->packetNum();
//        qDebug() << "vNum :" << vNum << "==== aNum : " << aNum;
        if(vNum >= 50 || aNum >= 50){
            av_usleep(100000);
            continue;
        }
        m_mutex.lock();
        m_packet_readEnd = false;
        if(av_read_frame(m_formatCtx,packet) < 0){
            m_packet_readEnd = true;
            m_mutex.unlock();
            break; // 认为数据已经读完了
        }
//        qDebug() << packet->stream_index << ":" << packet->pts << "---" << packet->dts;
        if(getPacketPts(packet) == AV_NOPTS_VALUE){
            this->freePacket(packet);
            m_mutex.unlock();
            continue;
        }
        if(packet->stream_index == m_videoStream){
            if(m_isVideoReady)m_video->addPacket(packet);
            else this->freePacket(packet);
        }else if(packet->stream_index == m_audioStream){
            if(m_isAudioReady)m_audio->addPacket(packet);
            else this->freePacket(packet);
        }else {
            c_log("av_read_frame : packet->stream_index error ,freePacket");
            this->freePacket(packet);
        }
        m_mutex.unlock();
    }
    if(m_isReading){
        m_video->waitEnd();
        m_audio->waitEnd();
    }else{
        m_video->stopNow();
        m_audio->stopNow();
    }
    if(m_CodecCtx_audio)avcodec_flush_buffers(m_CodecCtx_audio);
    if(m_CodecCtx_video)avcodec_flush_buffers(m_CodecCtx_video);
    if(m_buffer_audio){
        av_free(m_buffer_audio);
        m_buffer_audio = NULL;
    }
    if(m_frame_audio){
        av_free(m_frame_audio);
        m_frame_audio = NULL;
    }
    if(m_frame_video){
        av_free(m_frame_video);
        m_frame_video = NULL;
    }
    if(m_frameRGB){
        av_free(m_frameRGB);
        m_frameRGB = NULL;
    }
    if(m_swrCtr)swr_free(&m_swrCtr);
    if(m_CodecCtx_audio){
        avcodec_close(m_CodecCtx_audio);
        m_CodecCtx_audio = NULL;
        m_Codec_audio = NULL;
    }
    if(m_CodecCtx_video){
        avcodec_close(m_CodecCtx_video);
        m_CodecCtx_video = NULL;
        m_Codec_video = NULL;
    }
    if(m_audio_output){
        m_audio_output->stop();
        delete m_audio_output;
        m_audio_output = NULL;
        m_audio_IOdevice = NULL;
    }
    avformat_close_input(&m_formatCtx);
    emit this->videoEnd();
    m_isReading = false;
}

bool DecoderFile::openAudioDecoder()
{
    m_isAudioReady = false;
    if(m_audioStream == -1)return m_isAudioReady;

    m_CodecCtx_audio = m_formatCtx->streams[m_audioStream]->codec;
    m_Codec_audio = avcodec_find_decoder(m_CodecCtx_audio->codec_id);
    if(m_Codec_audio == NULL){
        c_log("can not find audio decoder");
        return m_isAudioReady;
    }
    if(avcodec_open2(m_CodecCtx_audio,m_Codec_audio,NULL) < 0 ){
        c_log("can not open audio decoder");
        return m_isAudioReady;
    }
    int sample_size = av_samples_get_buffer_size(NULL,1,1,CAUDIO_SAMPLE_SIZE,1)*8;
    if(!creatAudioOutput(m_CodecCtx_audio->sample_rate,m_CodecCtx_audio->channels,sample_size)){
        return m_isAudioReady;
    }
    m_frame_audio = av_frame_alloc();
    int in_channel_layout = av_get_default_channel_layout(m_CodecCtx_audio->channels);
    int out_channel_layout = in_channel_layout;
    m_swrCtr = swr_alloc_set_opts(NULL,out_channel_layout,CAUDIO_SAMPLE_SIZE,m_CodecCtx_audio->sample_rate,
                       in_channel_layout,m_CodecCtx_audio->sample_fmt,m_CodecCtx_audio->sample_rate,0,NULL);
    swr_init(m_swrCtr);
    m_buffer_audio =(uint8_t *)av_malloc(CAUDIO_BUFFER_SIZE);

    m_isAudioReady = true;
    m_audio->startRun();
    return m_isAudioReady;
}

bool DecoderFile::openVideoDecoder()
{
    m_isVideoReady = false;
    if(m_videoStream == -1)return m_isVideoReady;

    m_CodecCtx_video = m_formatCtx->streams[m_videoStream]->codec;
    m_Codec_video = avcodec_find_decoder(m_CodecCtx_video->codec_id);
    if(m_CodecCtx_video == NULL){
        c_log("can not find video decoder");
        return m_isVideoReady;
    }
    if(avcodec_open2(m_CodecCtx_video,m_Codec_video,NULL) < 0 ){
        c_log("can not open video decoder");
        return m_isVideoReady;
    }
    m_frame_video = av_frame_alloc();
    m_frameRGB    = av_frame_alloc();

    m_video->startRun();
    m_isVideoReady = true;
    return m_isVideoReady;
}

int DecoderFile::writeAudioData()
{
    if(!m_audio_output || !m_audio_IOdevice) return 0;
    int buff_size = av_samples_get_buffer_size(NULL,m_CodecCtx_audio->channels,m_frame_audio->nb_samples,CAUDIO_SAMPLE_SIZE,1);
    int ret = m_audio_IOdevice->write((char *)m_buffer_audio,buff_size);

    return ret;
}

void DecoderFile::getStreamTimeBase()
{
    int t_index = -1;
    if(m_videoStream != -1)t_index = m_videoStream;
    else if (m_audioStream != -1)t_index = m_audioStream;
    else return;
    m_stream_timebase = m_formatCtx->streams[t_index]->time_base.num*1.0/m_formatCtx->streams[t_index]->time_base.den*1.0;
}

int64_t DecoderFile::getPacketPts(AVPacket *pkt)
{
    int64_t pts  = AV_NOPTS_VALUE;
    if(!pkt)return pts;

    if(pkt->dts != AV_NOPTS_VALUE)pts = pkt->dts;
    else if(pkt->pts != AV_NOPTS_VALUE)pts = pkt->pts;

    return pts;
}

void DecoderFile::freePacket(AVPacket *pkt)
{
    av_packet_unref(pkt);
}

bool DecoderFile::creatAudioOutput(int sample_rate, int channels, int sample_size)
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
        c_log("audio format not supported");
        return false;
    }
    m_audio_output = new QAudioOutput(format);
    m_audio_output->setVolume(m_volume);
    m_audio_IOdevice = m_audio_output->start();
    return true;
}

void DecoderFile::currentUITime(int64_t pts)
{
    if(pts == AV_NOPTS_VALUE)return;
    if(m_firstPts != AV_NOPTS_VALUE){
        int ctime = (pts-m_firstPts)*m_stream_timebase;
        emit videoPts(ctime);
    }else {
        m_firstPts = pts;
    }

    if(m_count_BasePts == AV_NOPTS_VALUE){
        m_count_BasePts = pts;
        m_count_BaseTime = av_gettime();
//        qDebug() << "pts ===========" << pts;
    }
}




