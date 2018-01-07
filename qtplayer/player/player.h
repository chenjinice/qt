#ifndef PLAYER_H
#define PLAYER_H

class QImage;
class ReadThread;
class QPushButton;

#include <QWidget>

class Player : public QWidget
{
    Q_OBJECT
public:
    explicit Player(QWidget *parent = nullptr);
    Player(const QString path, QWidget *parent = nullptr);
    ~Player();

    void setPath(const QString path);
    void start();
    void stop();

protected:
    QImage      m_image;
    ReadThread  *m_readThread;
    QPushButton *m_startButton;
    QPushButton *m_stopButton;
    bool        m_playing;

    void initParam();
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
public slots:
    void showFrame(QImage img);
};

#endif // PLAYER_H
