#ifndef WHOCHAT_SERVER_H
#define WHOCHAT_SERVER_H

#include "../common/common.h"

using namespace std;

class Server {

public:
    Server();
    void Init();
    void Close();
    void Start();

private:
    int SendBroadcastMessage(int client_fd);
    struct sockaddr_in server_addr;

    // listener to socket
    int server_socket_fd;
    // created epoll handler
    int epoll_fd;
    
    list<int> clients_list;
}

#endif