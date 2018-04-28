#ifndef DecoderUdp_H
#define DecoderUdp_H

class QThread;
class AVFormatContext;
class AVPacket;
class AVCodecContext;
class AVCodec;
class AVFrame;
class SwrContext;
class QAudioOutput;
class QIODevice;
class VideoUdp;
class AudioUdp;

#include "../decoder.h"

class DecoderUdp : public Decoder
{
    Q_OBJECT
    friend VideoUdp;
    friend AudioUdp;
public:
    DecoderUdp();
    ~DecoderUdp();

    void setUrl(const QString &path);
    void startPlay();
    void stop();
    void pause();
    void setVolume(int volume);
    void seek(int pos);
    bool isReading();

protected:

    VideoUdp *m_video;
    AudioUdp *m_audio;
    QThread  *m_thread_read;
    QString   m_file_path;

    int      m_videoStream;
    int      m_audioStream;

    bool     m_isReading;
    bool     m_isPause;
    bool     m_isAudioReady;
    bool     m_isVideoReady;

    AVFormatContext *m_formatCtx;
    AVCodecContext  *m_CodecCtx_video;
    AVCodecContext  *m_CodecCtx_audio;
    AVCodec         *m_Codec_video;
    AVCodec         *m_Codec_audio;
    AVFrame         *m_frame_video;
    AVFrame         *m_frameRGB;
    AVFrame         *m_frame_audio;
    SwrContext      *m_swrCtr;
    uint8_t         *m_buffer_audio;

    double           m_volume;
    QAudioOutput    *m_audio_output;
    QIODevice       *m_audio_IOdevice;

    double           m_stream_timebase;
    int64_t          m_firstPts;
    int64_t          m_count_BasePts;
    int64_t          m_count_BaseTime;

    void initParam();
    void startReadThread();
    bool openAudioDecoder();
    bool openVideoDecoder();
    int  writeAudioData();
    void getStreamTimeBase();
    int64_t getPacketPts(AVPacket *pkt);
    void freePacket(AVPacket *pkt);
    bool creatAudioOutput(int sample_rate , int channels, int sample_size);
    void currentUITime(int64_t pts);
};

#endif // DecoderUdp_H
