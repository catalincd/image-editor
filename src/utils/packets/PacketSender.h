#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "../OSUtils.h"
#include "../types.h"

class PacketSender
{
    int m_session;
    int m_socket;
    int m_lastSent;
    size_t m_totalSent;
    Buffer m_buffer;
    Buffer m_received;

public:
    PacketSender(int socket, const Buffer& data);
    bool Send();
    ~PacketSender();

private:
    bool SendBytes(Buffer);
    bool ReceiveBytes(size_t);
};