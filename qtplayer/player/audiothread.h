#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H

class AVPacket;
class ReadThread;
class QAudioOutput;
class QIODevice;

#include <QThread>

class AudioThread : public QThread
{
public:
    AudioThread(ReadThread *pthread);
    ~AudioThread();

    void addPacket(AVPacket *pkt);
    int packetNum();
    void clear();
    void stopNow();
    void waitStop();
    void start(Priority pty = InheritPriority);

protected:
    bool               m_running;
    bool               m_audio_end;
    bool               m_error;
    QList<AVPacket>    m_list;
    ReadThread        *m_readThread;
    QAudioOutput      *m_audio_output;
    QIODevice         *m_audio_IOdevice;

    void run();
    bool creatAudioOutput(int sample_rate , int channels, int sample_size);
    void freeList();
};

#endif // AUDIOTHREAD_H
