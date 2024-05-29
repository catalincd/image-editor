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


class PacketSender
{
    int m_socket;
    int m_packets_num;
    int m_sent_count;
    int m_ack_count;
    int m_total_sent_count;
    int m_session;
    Buffer m_buffer;
    std::mutex m_lock;
    std::thread m_thread;
    std::vector<Packet> m_packets;
    std::vector<bool> m_ack;

public:
    PacketSender(int socket, Buffer buffer);
    bool Send();
    int GetSocket();
    void StartAccepting();
    ~PacketSender();
};