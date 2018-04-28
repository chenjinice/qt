#ifndef VideoUdp_H
#define VideoUdp_H

class DecoderUdp;
class QThread;
class AVPacket;

#include <QObject>
#include <QMutex>

class VideoUdp : public QObject
{
    Q_OBJECT
public:
    explicit VideoUdp(DecoderUdp *decoder);
    ~VideoUdp();

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

#endif // VideoUdp_H
