/*
 *
 * 解码类的抽象类
 *
 */
#ifndef DECODER_H
#define DECODER_H

#include <QObject>
#include <QImage>

#define CAUDIO_BUFFER_SIZE 192000
#define CAUDIO_SAMPLE_SIZE AV_SAMPLE_FMT_S16

class Decoder : public QObject
{
    Q_OBJECT
public:
    explicit Decoder(){};

    virtual void setUrl(const QString &path) = 0;
    virtual void startPlay() = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void setVolume(int volume) = 0;
    virtual void seek(int pos) = 0;
    virtual bool isReading() = 0;

signals:
    void getFrame(QImage);
    void videoLength(int length);
    void videoPts(int length);
    void videoEnd();
};

#endif // DECODER_H
