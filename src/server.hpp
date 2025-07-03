#pragma once
#include <string>

class Server {
public:
    Server(int port);
    void run();

private:
    int port;
    int server_fd;
    int epoll_fd;

    void setup_server_socket();
    void event_loop();
    void handle_connection(int client_fd);
};