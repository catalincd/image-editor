#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
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

void SocketHandler::SetupConnection(int port, int clients)
{
    m_port = port;
    m_clients = clients;

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
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

    printf("Started listening on port %d\n", m_port);
    printf("Max clients %d\n", m_clients);

    ThreadHandler thr_pool(thread::hardware_concurrency());

    while (true)
    {
        m_clilen = sizeof(m_cli_addr);

        m_server_socket = accept(m_socket, (struct sockaddr *)&m_cli_addr, &m_clilen);
        if (m_server_socket < 0)
            Error("ERROR on accept");

        printf("Accepted connection: %d\n", m_server_socket);

        thr_pool.enqueue([=]
                         { ConnectionHandler::HandleConnection(m_server_socket); });
        sleep(0.01);
        // close(m_server_socket);
    }

    close(m_socket);
}