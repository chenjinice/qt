#ifndef PLAYER_H
#define PLAYER_H

class QImage;
class Decoder;
class QPushButton;
class QLabel;
class QWidget;
class MySlider;

#include <QWidget>

class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player( QWidget *parent = nullptr);
    ~Player();

    void setUrl(const QString &url);
    void setVolume(int volume);
    void startPlay();
    bool isPlaying();
    void pause();
    void stop();
    void seek(int pos);

protected:
    QWidget     *m_controller;
    QImage      m_image;
    QLabel      *m_currentTime;
    QLabel      *m_videoLength;
    MySlider    *m_slider;
    QPushButton *m_startButton;
    QPushButton *m_pauseButton;
    QPushButton *m_stopButton;

    Decoder     *m_decoder;

    void createController();
    int sceneHeight();
    QSize sceneSize();
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);

    void setVideoLength(int length);
    void setVideoCurrentTime(int length);
    void showFrame(QImage img);
    void decoderEnd();

public slots:

};

#endif // PLAYER_H
