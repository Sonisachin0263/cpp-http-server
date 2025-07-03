#include "server.hpp"
#include "thread_pool.hpp"
#include "http.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>
#include <iostream>

#define MAX_EVENTS 1000

ThreadPool pool(4);

Server::Server(int port) : port(port) {}

void Server::setup_server_socket() {
    server_fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd, SOMAXCONN);

    epoll_fd = epoll_create1(0);

    epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev);
}

void Server::run() {
    setup_server_socket();
    event_loop();
}

void Server::event_loop() {
    epoll_event events[MAX_EVENTS];

    while (true) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == server_fd) {
                int client_fd = accept4(server_fd, nullptr, nullptr, SOCK_NONBLOCK);
                epoll_event ev{};
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = client_fd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
            } else {
                int client_fd = events[i].data.fd;
                pool.enqueue([client_fd]() {
                    char buffer[4096];
                    ssize_t bytes = recv(client_fd, buffer, sizeof(buffer), 0);
                    if (bytes > 0) {
                        std::string request(buffer, bytes);
                        std::string response = build_http_response(request);
                        send(client_fd, response.c_str(), response.size(), 0);
                    }
                    close(client_fd);
                });
            }
        }
    }
}