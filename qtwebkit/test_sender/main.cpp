#include <unistd.h>
#include <iostream>

#include "server.h"


using namespace std;

int main()
{
    Server vision(12347,(char *)"vision");
    Server fusion(12348,(char *)"fusion");
    Server radar(12346,(char *)"radar");

    while(1){
        vision.send();
        fusion.send();
        radar.send();
        usleep(100000);
    }

    return 0;
}

