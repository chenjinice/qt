#include "player.h"
#include "readthread.h"
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QCloseEvent>
#include <QPushButton>
#include <QBoxLayout>

Player::Player(QWidget *parent) : QWidget(parent)
{
    this->initParam();
}

Player::Player(const QString path, QWidget *parent) : QWidget(parent)
{
    this->initParam();
    this->setPath(path);
}

Player::~Player()
{
    this->stop();
    if(m_readThread){
        delete m_readThread;
        m_readThread = NULL;
    }
}

void Player::setPath(const QString path)
{
    m_readThread->setFilePath(path);
}

void Player::start()
{
    if(m_playing)return;
    m_playing = true;
    m_readThread->start();

}

void Player::stop()
{
    m_playing = false;
    update();
    if(m_readThread){
        m_readThread->stop();
    }
}

void Player::initParam()
{
    m_playing = false;
    m_readThread = NULL;
    m_readThread = new ReadThread;
    connect(m_readThread,&ReadThread::getFrame,this,&Player::showFrame);

    m_startButton = new QPushButton(tr("开始"),this);
    m_stopButton  = new QPushButton(tr("停止"),this);
    m_startButton->setMaximumWidth(100);
    m_stopButton->setMaximumWidth(100);
    connect(m_startButton,&QPushButton::clicked,this,&Player::start);
    connect(m_stopButton,&QPushButton::clicked,this,&Player::stop);
    QHBoxLayout *layout = new QHBoxLayout;
    this->setLayout(layout);
    layout->addWidget(m_startButton);
    layout->addWidget(m_stopButton);
}

void Player::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0,0,this->width(),this->height());
    if(m_image.width() <=0 || !m_playing){
        m_image = QImage(0,0,QImage::Format_Alpha8);
        return;
    }
    QImage img;
    if(this->width() < m_image.width() || this->height() < m_image.height()){
        img = m_image.scaled(this->size(),Qt::KeepAspectRatio);
    }else {
        img = m_image;
    }

    int x = this->width()-img.width();
    int y = this->height()-img.height();
    x/=2;
    y/=2;
    painter.drawImage(QPoint(x,y),img);
}

void Player::closeEvent(QCloseEvent *event)
{
    this->stop();
    event->accept();
}

void Player::showFrame(QImage img)
{
    m_image = img;
    this->update();
}

