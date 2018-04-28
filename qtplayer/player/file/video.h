#ifndef VIDEO_H
#define VIDEO_H

class DecoderFile;
class QThread;
class AVPacket;

#include <QObject>
#include <QMutex>

class Video : public QObject
{
    Q_OBJECT
public:
    explicit Video(DecoderFile *decoder);
    ~Video();

    void addPacket(AVPacket *pkt);
    void startRun();
    int  packetNum();
    void stopNow();
    void waitEnd();
    void clearList();

protected:
    bool             m_isRunning;
    bool             m_isEnd;
    QMutex           m_mutex;
    QThread         *m_thread;
    DecoderFile     *m_decoder;
    QList<AVPacket>  m_list;

    void startThread();
    int getOnePacket(AVPacket *pkt);
    void freeList();
};

#endif // VIDEO_H
