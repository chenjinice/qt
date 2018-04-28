#include "player.h"
#include "plugin/myslider.h"
#include "plugin/clog.h"
#include "file/decoderfile.h"
#include "udp/decoderudp.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QCloseEvent>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QThread>

#define DEFAULT_TIME "00:00:00"

Player::Player(QWidget *parent) : QWidget(parent)
{
    m_decoder = NULL;
    this->createController();
}

Player::~Player()
{
//    qDebug() << "~~~player";
    if(m_decoder){
        m_decoder->stop();
        delete m_decoder;
        m_decoder = NULL;
    }
}

void Player::setUrl(const QString &url)
{
    if(m_decoder){
        m_decoder->disconnect(this);
        delete m_decoder;
        m_decoder = NULL;
    }
    if(url.startsWith("udp:",Qt::CaseInsensitive)){
        m_decoder = new DecoderUdp;
        connect(m_decoder,&Decoder::getFrame,this,&Player::showFrame);
        connect(m_decoder,&Decoder::videoEnd,this,&Player::decoderEnd);
        connect(m_decoder,&Decoder::videoPts,this,&Player::setVideoCurrentTime);
    }else {
        m_decoder = new DecoderFile;
        connect(m_decoder,&Decoder::getFrame,this,&Player::showFrame);
        connect(m_decoder,&Decoder::videoEnd,this,&Player::decoderEnd);
        connect(m_decoder,&Decoder::videoPts,this,&Player::setVideoCurrentTime);
        connect(m_decoder,&Decoder::videoLength,this,&Player::setVideoLength);
    }
    if(m_decoder){
        m_decoder->setUrl(url);
    }
}

void Player::setVolume(int volume)
{
    if(m_decoder)m_decoder->setVolume(volume);
}

void Player::startPlay()
{
    if(m_decoder)m_decoder->startPlay();
}

bool Player::isPlaying()
{
    if(m_decoder)return m_decoder->isReading();
}

void Player::pause()
{
    if(m_decoder)m_decoder->pause();
}

void Player::stop()
{
    if(m_decoder)m_decoder->stop();
}

void Player::seek(int pos)
{
//    qDebug() << pos;
    if(m_decoder)m_decoder->seek(pos);
}

void Player::createController()
{
    m_controller = new QWidget(this);
    m_controller->setStyleSheet("background-color:rgb(255,255,255)");
    m_controller->setMaximumHeight(40);
    QGridLayout *layout = new QGridLayout;
    layout->setContentsMargins(15,0,15,0);
    layout->setSpacing(2);
    m_controller->setLayout(layout);

    m_startButton = new QPushButton(tr("开始"),m_controller);
    m_pauseButton = new QPushButton(tr("暂停"),m_controller);
    m_stopButton  = new QPushButton(tr("停止"),m_controller);
    m_slider = new MySlider(m_controller);
    m_slider->setMaximum(0);
    m_currentTime = new QLabel(m_controller);
    m_currentTime->setText(DEFAULT_TIME);
    m_videoLength = new QLabel(m_controller);
    m_videoLength->setText(DEFAULT_TIME);
    m_startButton->setMaximumWidth(50);
    m_stopButton->setMaximumWidth(50);
    m_pauseButton->setMaximumWidth(50);
    QLabel *label = new QLabel(m_controller);
    label->setText("/");

    connect(m_startButton,&QPushButton::clicked,this,&Player::startPlay);
    connect(m_pauseButton,&QPushButton::clicked,this,&Player::pause);
    connect(m_stopButton,&QPushButton::clicked,this,&Player::stop);
    connect(m_slider,&MySlider::clickedPosition,this,&Player::seek);

    layout->addWidget(m_startButton,0,0,1,1);
    layout->addWidget(m_pauseButton,0,1,1,1);
    layout->addWidget(m_stopButton,0,2,1,1);
    layout->addWidget(m_slider,0,3,1,1);
    layout->addWidget(m_currentTime,0,4,1,1);
    layout->addWidget(label,0,5,1,1);
    layout->addWidget(m_videoLength,0,6,1,1);

    QLabel *scene = new QLabel(this);
    QGridLayout *mlayout = new QGridLayout;
    mlayout->setMargin(0);
    mlayout->setSpacing(0);
    this->setLayout(mlayout);
    mlayout->addWidget(scene,0,0,1,1);
    mlayout->addWidget(m_controller,1,0,1,1);
}

int Player::sceneHeight()
{
    int c_height = (m_controller && !m_controller->isHidden()) ? m_controller->height():0;
    return this->height()-c_height;
}

QSize Player::sceneSize()
{
    QSize size(this->width(),this->sceneHeight());
    return size;
}

void Player::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0,0,this->width(),this->sceneHeight());
    if(m_image.width() <= 0 || !m_decoder->isReading()){
        m_image = QImage(0,0,QImage::Format_Alpha8);
        return;
    }
    QImage img;
//    if(this->width() < m_image.width() || this->sceneHeight() < m_image.height()){
        img = m_image.scaled(this->sceneSize(),Qt::KeepAspectRatio);
//    }else {
//        img = m_image;
//    }

    int x = this->width()-img.width();
    int y = this->sceneHeight()-img.height();
    x/=2;
    y/=2;
    painter.drawImage(QPoint(x,y),img);
}

void Player::closeEvent(QCloseEvent *event)
{
//     qDebug() << "~~~player closeevent";
    if(m_decoder){
        m_decoder->stop();
        delete m_decoder;
        m_decoder = NULL;
    }
    event->accept();
}

void Player::setVideoLength(int length)
{
    if(m_controller->isHidden())return;
    QString m = QString::number(length/60%60);
    QString h = QString::number(length/3600);
    QString s = QString::number(length%60);
    QString str = QString("%1:%2:%3").arg(h,2,'0').arg(m,2,'0').arg(s,2,'0');
    m_videoLength->setText(str);
    m_slider->setMaximum(length-1);
}

void Player::setVideoCurrentTime(int length)
{
    QString m = QString::number(length/60%60);
    QString h = QString::number(length/3600);
    QString s = QString::number(length%60);
    QString str = QString("%1:%2:%3").arg(h,2,'0').arg(m,2,'0').arg(s,2,'0');
    m_currentTime->setText(str);
    m_slider->setValue(length);
}

void Player::showFrame(QImage img)
{
    m_image = img;
    this->update();
}

void Player::decoderEnd()
{
    m_slider->setMaximum(0);
    m_videoLength->setText(DEFAULT_TIME);
    m_currentTime->setText(DEFAULT_TIME);
    update();
}

