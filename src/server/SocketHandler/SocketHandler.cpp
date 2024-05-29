#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "SocketHandler.h"
#include "../ThreadHandler/ThreadHandler.h"
#include "../ConnectionHandler/ConnectionHandler.h"
#include "../../utils/OSUtils.h"

extern size_t MAX_PAYLOAD;

SocketHandler &SocketHandler::Instance()
{
    static SocketHandler instance;
    return instance;
}

void set_non_blocking(int socket)
{
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1)
    {
        Error("ERROR getting socket flags");
    }
    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        Error("ERROR setting socket to non-blocking");
    }
}

void SocketHandler::SetupConnection(int port, int clients)
{
    m_port = port;
    m_clients = clients;

    m_socket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (m_socket < 0)
        Error("ERROR opening socket");

    int optval = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    m_serv_addr.sin_family = AF_INET;
    m_serv_addr.sin_addr.s_addr = INADDR_ANY;
    m_serv_addr.sin_port = htons(m_port);

    if (bind(m_socket, (const sockaddr *)&m_serv_addr, sizeof(m_serv_addr)) < 0)
        Error("ERROR on binding");

    listen(m_socket, m_clients);

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        Error("ERROR creating epoll instance");
    }

    struct epoll_event event;
    event.data.fd = m_socket;
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_socket, &event) == -1)
    {
        Error("ERROR adding server socket to epoll");
    }

    // printf("Started listening on port %d\n", m_port);
    // printf("Max clients %d\n", m_clients);

    ThreadHandler thr_pool(thread::hardware_concurrency());

    std::vector<struct epoll_event> events(4);

    while (true)
    {
        int num_fds = epoll_wait(epoll_fd, events.data(), 4, -1);
        if (num_fds == -1)
        {
            if (errno == EINTR)
            {
                continue; // Restart if interrupted by signal
            }
            else
            {
                Error("ERROR in epoll_wait");
            }
        }

        for (int i = 0; i < num_fds; ++i)
        {
            if (events[i].data.fd == m_socket)
            {
                while (true)
                {
                    m_clilen = sizeof(m_cli_addr);
                    int client_socket = accept(m_socket, (struct sockaddr *)&m_cli_addr, &m_clilen);
                    if (client_socket == -1)
                    {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                        {
                            // No more incoming connections
                            continue;
                        }
                        else
                        {
                            Error("ERROR accept");
                            break;
                        }
                        printf("Accepted connection: %d\n", client_socket);
                    }
                    set_non_blocking(client_socket);
                    event.data.fd = client_socket;
                    event.events = EPOLLIN | EPOLLET;

                    thr_pool.enqueue([=]
                                 { ConnectionHandler::HandleConnection(client_socket); });
                                 
                    // if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event) == -1)
                    // {
                    //     perror("ERROR adding client socket to epoll");
                    //     close(client_socket);
                    // }
                }
            }
        }
        //sleep(0.001); // THIS IS MY LAST RESORT
        close(m_socket);
    }
}