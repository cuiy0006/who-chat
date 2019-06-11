#ifndef WHOCHAT_CLIENT_H
#define WHOCHAT_CLIENT_H

#include "common.h"
#include "string.h"

class Client{

public:
    Client();
    void close();
    void start();

private:
    void connect();
    int server_socket_fd;
    int pid;
    int epoll_fd;
    int pip_fd[2];
    bool is_client_work;
    char message[BUF_SIZE];
    struct sockaddr_in server_addr;

};



#endif