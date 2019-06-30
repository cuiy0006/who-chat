#include "client.h"


Client::Client(){
    server_addr.sin_family = PF_INET; //tcp/udp
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    server_socket_fd = 0;
    pid = 0;
    is_client_work = true;
    epoll_fd = 0;
}

void Client::Connect(){
    std::cout << "Initialzing server...\n";

    std::cout << "connect server(" << SERVER_IP << ":" << SERVER_PORT << ")...\n";

    server_socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if(server_socket_fd < 0){
        std::cerr << "create server socket fd error" << "\n";
        throw runtime_error("create server socket fd error");
    }

    std::cout << "server socket fd (" << server_socket_fd << ") created " << "\n";

    if(connect(server_socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
        std::cerr << "connect server error" << "\n";
        throw runtime_error("connect server error");
    }

    std::cout << "server connected" << "\n";

    // pipe_fd[0]: parent pipe
    // pipe_fd[1]: child pipe
    if(pipe(pipe_fd) < 0){
        std::cerr << "create pipe error" << "\n";
        throw runtime_error("create pipe error");
    }

    epoll_fd = epoll_create(EPOLL_SIZE);
    if(epoll_fd < 0){
        std::cerr << "create epoll fd error" << "\n";
        throw runtime_error("create epoll fd error");
    }

    std::cout << "epoll fd(" << epoll_fd << ") created\n";

    register_fd(epoll_fd, server_socket_fd, true);
    register_fd(epoll_fd, pipe_fd[0], true);

    std::cout << "Initialzed client\n";
}


void Client::Start(){
    static struct epoll_event events[2];
    Connect();
    pid = fork();
    if(pid < 0){
        std::cerr << "create child process error" << "\n";
        throw runtime_error("create child process error");
    }

    if(pid == 0){
        close(pipe_fd[0]);
        std::cout << "Please input 'exit' to exit the chat room\n";

        while(is_client_work){
            bzero(message, BUF_SIZE);
            fgets(message, BUF_SIZE, stdin);

            if(strncasecmp(message, EXIT_MESSAGE, strlen(EXIT_MESSAGE)) == 0){
                is_client_work = 0;
            } else {
                if(write(pipe_fd[1], message, strlen(message) - 1) < 0){
                    std::cerr << "write message error" << "\n";
                    throw runtime_error("write message error");
                }
            }
        }
    } else {
        close(pipe_fd[1]);
        while(is_client_work) {
            int epoll_events_count = epoll_wait(epoll_fd, events, 2, -1);

            for(int i = 0; i < epoll_events_count; i++){
                bzero(message, BUF_SIZE);

                if(events[i].data.fd == server_socket_fd){
                    int ret = recv(server_socket_fd, message, BUF_SIZE, 0);

                    if(ret == 0){
                        std::cout << "server connection closed: " << events[i].data.fd << "\n";
                        close(server_socket_fd);
                        is_client_work = 0;
                    } else {
                        std::cout << message << "\n";
                    }
                } else {
                    int ret = read(events[i].data.fd, message, BUF_SIZE);

                    if(ret == 0){
                        std::cout << "pipe closed: " << events[i].data.fd << "\n";
                        is_client_work = 0;
                    } else {
                        send(server_socket_fd, message, BUF_SIZE, 0);
                    }
                }
            }
        }
    }
}


void Client::Close(){
    if(pid){
        //close parent pipe
        close(pipe_fd[0]);
        close(server_socket_fd);
    } else {
        //close child pipe
        close(pipe_fd[1]);
    }
}


