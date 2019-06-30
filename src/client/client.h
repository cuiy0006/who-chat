#ifndef WHOCHAT_CLIENT_H
#define WHOCHAT_CLIENT_H

#include "common.h"
#include "string.h"

class Client{

public:
    Client();
    void Close();
    void Start();

private:
    void Connect();
    int server_socket_fd;
    int pid;
    int epoll_fd;
    int pipe_fd[2];
    bool is_client_work;
    char message[BUF_SIZE];
    struct sockaddr_in server_addr;

};



#endif