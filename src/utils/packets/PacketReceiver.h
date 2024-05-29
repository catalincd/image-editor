#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <getopt.h>
#include <chrono>
#include <arpa/inet.h>
#include "../types.h"
#include "../OSUtils.h"
#include <thread>     
#include <mutex>          


class PacketReceiver
{
    int m_socket;
    int m_packets_num;
    int m_received_count;
    int m_session;
    bool m_init;

    Buffer m_buffer;
    std::mutex m_lock;
    std::thread m_thread;
    std::vector<bool> m_ack;

public:
    PacketReceiver(int socket);
    bool Receive();
    Buffer& GetBuffer(); 
    ~PacketReceiver();
private:
    void Init(Packet packet);
    void SendAck(Packet packet);
};