#ifndef AudioUdp_H
#define AudioUdp_H

class DecoderUdp;
class QThread;
class AVPacket;

#include <QObject>
#include <QMutex>

class AudioUdp : public QObject
{
    Q_OBJECT
public:
    explicit AudioUdp(DecoderUdp *decoder);
    ~AudioUdp();

    void addPacket(AVPacket *pkt);
    void startRun();
    int  packetNum();
    void stopNow();
    void clearList();
    void freeSome(int num);

protected:
    bool             m_isRunning;
    QMutex           m_mutex;
    QThread         *m_thread;
    DecoderUdp      *m_decoder;
    QList<AVPacket>  m_list;

    void startThread();
    int getOnePacket(AVPacket *pkt);
    void freeList();
};

#endif // AudioUdp_H
