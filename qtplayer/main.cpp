#include "player/player.h"
#include <QApplication>
#include <QGridLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget w;
    w.setMinimumWidth(500);
    w.setMinimumHeight(500);
    QGridLayout layout;
    w.setLayout(&layout);

    // udp://233.233.233.223:7777 ///home/chen/v/ghost.rmvb joke.flv song.f4v
    // ?overrun_nonfatal=1&fifo_size=139438

    Player p("udp://233.233.233.223:7777?overrun_nonfatal=1&fifo_size=139438");
    p.start();
    p.show();

    Player p1("/home/chen/v/joke.flv");
    p1.start();
    p1.show();

//    Player p2("/home/chen/v/song.f4v");
//    p2.start();
//    p2.show();

//    Player p3("udp://233.233.233.223:7777");
//    p3.start();
//    p3.show();

    layout.addWidget(&p,0,0,1,1);
    layout.addWidget(&p1,0,1,1,1);
//    layout.addWidget(&p2,1,0,1,1);
//    layout.addWidget(&p3,1,1,1,1);
    w.show();

//    Player p4("/home/chen/v/ghost.rmvb");
//    p4.start();
//    p4.show();

    return a.exec();
}
