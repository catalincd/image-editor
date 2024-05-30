#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ConnectionHandler.h"
#include "../ImageEditor/ImageEditor.h"
#include "../../utils/OSUtils.h"

#include "../../utils/packets/PacketSender.h"
#include "../../utils/packets/PacketReceiver.h"

void SwapBytes(void *pv, size_t n)
{
    char *p = (char *)pv;
    size_t lo, hi;
    for (lo = 0, hi = n - 1; hi > lo; lo++, hi--)
    {
        char tmp = p[lo];
        p[lo] = p[hi];
        p[hi] = tmp;
    }
}
#define SWAP(x) SwapBytes(&x, sizeof(x));

ConnectionHandler &ConnectionHandler::Instance()
{
    static ConnectionHandler instance;
    return instance;
}

extern size_t MAX_PAYLOAD;

void ConnectionHandler::HandleConnection(int socket)
{
    Buffer receivedBuffer;

    PacketReceiver packetReceiver(socket);
    if(packetReceiver.Receive())
    {
        receivedBuffer = packetReceiver.GetBuffer();
        std::cout << "Received " << receivedBuffer.size << "B" << std::endl;
    }
    else
    {
        std::cerr << "FAILED packetReceiver" << std::endl;
        return;
    }


    Payload payload = BytesToPayload(receivedBuffer);
    payload.image = ImageEditor::Instance().Edit(payload); // delete old image so we don't leak mem? nah overrated
    Buffer response_payload = PayloadToBytes(payload);
    // TODO: HANDLE BIZONIC ERRORS
    printf("Server: Edited image %d bytes\n", payload.image.size);

    PacketSender packetSender(socket, response_payload);

    if(packetSender.Send())
    {
        std::cout << "Sent " << response_payload.size << "B" << std::endl;
    }
    else
    {
        std::cerr << "FAILED PacketSender" << std::endl;
    }

    free(payload.image.data);
    
    close(socket);
}
