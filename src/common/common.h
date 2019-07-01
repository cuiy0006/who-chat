#ifndef WHOCHAT_COMMON_H
#define WHOCHAT_COMMON_H

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using std::runtime_error;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
// max handlers epoll supports
#define EPOLL_SIZE 5000
// buffer size 65535
#define BUF_SIZE 0XFFFF
// maximum length to which the queue of pending connections for sockfd may grow
// client get error if exceed
#define MAX_CON_LENGTH 5

// common message
#define SERVER_WELCOME_MESSAGE "Welcome to char, your chat id: %d"
#define EXIT_MESSAGE "End chat."
#define LAST_USER_MESSAGE "You are the only user in the chat."
#define SERVER_MESSAGE "ClientId [%d] say >> %s"

// register socket fd to epoll
// epoll_fd: handler of epoll
// fd: handler which is listened to
// enable_et: edge-triggered
static void register_fd(int epoll_fd, int fd, bool enable_et) {
    // struct epoll_event
    // {
    //        uint32_t events;	/* Epoll events */
    //        epoll_data_t data;	/* User data variable */
    // };
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if(enable_et){
        event.events |= EPOLLET;
    }
    // register socket fd to epoll_fd
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event);
    // set nonblock
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
    std::cout << "fd[" << fd << "] is registered to epoll[" << epoll_fd << "]" << std::endl; 
}

#endif