#ifndef SERVER_H
#define SERVER_H

#include <iostream>

class Server
{
public:
    Server(uint16_t port,char *filter);
    ~Server();
    void send();

private:
    uint16_t 	 m_port;	//socket 端口
    void        *m_context;
    void        *m_publisher;
    char        m_filter[100];

    int set_vision(uint8_t *buffer);
    int set_fusion(uint8_t *buffer);
    int set_radar(uint8_t *buffer);
};

#endif // SERVER_H
