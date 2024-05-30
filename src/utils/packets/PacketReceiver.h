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

class PacketReceiver
{
    int m_session;
    int m_socket;
    int m_receivedCount;
    size_t m_receivedSize;
    enum State {IDLE, RECEIVING, FULL, ENDED} m_state;
 
    Buffer m_buffer;
    Buffer m_received;

public:
    PacketReceiver(int socket);
    bool Receive();
    Buffer& GetBuffer();
    ~PacketReceiver();

private:
    bool SendBytes(Buffer);
    bool ReceiveBytes(size_t);
};