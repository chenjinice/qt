#ifndef AUDIO_H
#define AUDIO_H

class DecoderFile;
class QThread;
class AVPacket;

#include <QObject>
#include <QMutex>

class Audio : public QObject
{
    Q_OBJECT
public:
    explicit Audio(DecoderFile *decoder);
    ~Audio();

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

#endif // AUDIO_H
