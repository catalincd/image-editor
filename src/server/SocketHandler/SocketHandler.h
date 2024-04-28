#pragma once


class SocketHandler
{
    int m_clients;
    int m_port;
    int m_socket;
    int m_server_socket;
    struct sockaddr_in m_serv_addr, m_cli_addr;
    socklen_t m_clilen;
public:
    static SocketHandler& Instance();
    void SetupConnection(int port = 9090, int clients = 100);
};