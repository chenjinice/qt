#include "player/player.h"
#include <QApplication>
#include <QGridLayout>
#include <QSlider>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    w.setMinimumWidth(500);
    w.setMinimumHeight(500);
    QGridLayout layout;
    w.setLayout(&layout);

    QString v = "udp://233.233.233.223:7777?overrun_nonfatal=1&fifo_size=139438";
    QString v1 = "/home/chen/v/ghost.rmvb";
    QString v2 = "/home/chen/v/song.f4v";
    QString v3 = "/home/chen/v/joke.flv";
    QString v4 = "/home/chen/v/a.mp3";
    QString v5 = "/home/chen/v/cctv2-4.ts";

    Player p;
    p.setUrl(v2);
    p.startPlay();
    Player p1;
    p1.setUrl(v5);
    p1.startPlay();
    Player p2;
//    p2.setUrl(v);
//    p2.startPlay();
//    Player p3;
//    p3.setUrl(v);
//    p3.startPlay();

//    Player *pp = NULL;
//    for(int i=0;i<10000;i++){
//        if(i%2 == 0){
//            pp = new Player("/home/chen/v/song.f4v");
//            pp->start();
//            QThread::sleep(1);
//        }else {
//            delete pp;
//            pp = NULL;
//        }

//    }

    layout.addWidget(&p,0,0,1,1);
    layout.addWidget(&p1,0,1,1,1);
//    layout.addWidget(&p2,1,0,1,1);
//    layout.addWidget(&p3,1,1,1,1);
    w.show();

    QSlider slider(Qt::Orientation::Horizontal);
    slider.setMaximum(100);
    QObject::connect(&slider,&QSlider::valueChanged,&p,&Player::setVolume);
    layout.addWidget(&slider,5,0,1,1);

    return a.exec();
}
