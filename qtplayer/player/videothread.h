#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

class AVPacket;
class ReadThread;

#include <QThread>

class VideoThread : public QThread
{
public:
    VideoThread(ReadThread *pthread);
    ~VideoThread();

    void addPacket(AVPacket *pkt);
    int packetNum();
    void clear();
    void stopNow();
    void waitStop();
    void start(Priority pty = InheritPriority);

protected:
    bool               m_running;
    bool               m_video_end;
    bool               m_error;
    QList<AVPacket>    m_list;
    ReadThread        *m_readThread;

    void run();
    void freeList();
};

#endif // VIDEOTHREAD_H
