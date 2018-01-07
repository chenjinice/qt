#ifndef READTHREAD_H
#define READTHREAD_H

class AVFormatContext;
class VideoThread;
class AudioThread;

#include <QThread>
#include <QImage>

class ReadThread : public QThread
{
    Q_OBJECT
    friend VideoThread;
    friend AudioThread;
public:
    ReadThread();
    ~ReadThread();

    void setFilePath(const QString path);
    void start(Priority pty = InheritPriority);
    void stop();

protected:
    static int  m_count;
    QString     m_path;
    bool        m_reading;

    int              m_videoStream;
    int              m_audioStream;
    int64_t          m_startTime;
    int64_t          m_firstPts;
    double           m_timebase;
    AVFormatContext *m_formatCtx;
    VideoThread     *m_videoThread;
    AudioThread     *m_audioThread;

    void run();

signals:
    void getFrame(QImage);
};

#endif // READTHREAD_H
