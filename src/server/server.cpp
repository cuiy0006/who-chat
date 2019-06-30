#include "server.h"

Server::Server(){
    server_addr.sin_family = PF_INET; //tcp/udp
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    server_socket_fd = 0;
    epoll_fd = 0;
}

void Server::Init(){

    server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket_fd < 0){
        throw runtime_error("create server socket fd error");
    }

    if(bind(server_socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
        throw runtime_error("bind server socket fd error");
    }

    int ret = listen(server_socket_fd, MAX_CON_LENGTH); 
    if(ret < 0) {
        throw runtime_error("listen server socket fd error");
    }

    epoll_fd = epoll_create(EPOLL_SIZE);
    if(epoll_fd < 0){
        throw runtime_error("create epoll fd error");
    }

    register_fd(epoll_fd, server_socket_fd, true);
}

int Server::SendBroadcastMessage(int client_fd){
    // message
    char buf[BUF_SIZE];
    // formatted message
    char message[BUF_SIZE];

    bzero(buf, BUF_SIZE);
    bzero(message, BUF_SIZE);

    int len = recv(client_fd, buf, BUF_SIZE, 0);

    // client closed the connection
    if(len == 0){
        close(client_fd);
        clients_list.remove(client_fd);
    } else {
        if(clients_list.size() == 1){
            send(client_fd, LAST_USER_MESSAGE, strlen(LAST_USER_MESSAGE), 0);
            return len;
        }

        sprintf(message, SERVER_MESSAGE, client_fd, buf);

        auto begin = clients_list.begin();
        auto end = clients_list.end();
        for(auto it = begin; it != end; ++it){
            if(*it != client_fd){
                if(send(*it, message, BUF_SIZE, 0) < 0){
                    throw runtime_error("Cannot send message to a client");
                }
            }
        }
    }
    return len;
}

void Server::Start(){
    static struct epoll_event events[EPOLL_SIZE];

    Init();

    while(true){
        int epoll_events_count = epoll_wait(epoll_fd, events, EPOLL_SIZE, -1);

        if(epoll_events_count < 0){
            throw runtime_error("epoll wait error");
        }

        for(size_t i = 0; i < epoll_events_count; ++i){
            // the client's fd or server's fd
            int event_socket_fd = events[i].data.fd;

            if(event_socket_fd == server_socket_fd){
                struct sockaddr_in client_address;
                socklen_t client_address_size = sizeof(client_address);
                int client_socket_fd = accept(server_socket_fd, (struct sockaddr*) &client_address, &client_address_size);

                register_fd(epoll_fd, client_socket_fd, true);

                clients_list.push_back(client_socket_fd);

                char message[BUF_SIZE];
                bzero(message, BUF_SIZE);
                sprintf(message, SERVER_WELCOME_MESSAGE, client_socket_fd);
                int ret = send(client_socket_fd, message, BUF_SIZE, 0);
                if(ret < 0){
                    throw runtime_error("send welcome error.");
                }
            } else {
                int ret = SendBroadcastMessage(event_socket_fd);
                if(ret < 0){
                    throw runtime_error("broadcase error.");
                }
            }
        }
    }

    Close();
    
}

void Server::Close(){
    close(server_socket_fd);
    close(epoll_fd);
}