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

#include "../../utils/packets/PacketReceiver.h"
#include "../../utils/packets/PacketSender.h"

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
    PacketReceiver packetReceiver(socket);
    Buffer received_buffer;

    if(packetReceiver.Receive())
    {
        received_buffer = packetReceiver.GetBuffer();
        printf("Server: Received %d bytes\n", received_buffer.size);
    }
    else
    {
        printf("Server: Failed receiver");
        return;
    }
    
    Payload payload = BytesToPayload(received_buffer);
    payload.image = ImageEditor::Instance().Edit(payload); // delete old image so we don't leak mem? nah overrated
    Buffer response_payload = PayloadToBytes(payload);
    // TODO: HANDLE BIZONIC ERRORS
    printf("Server: Edited image %d bytes\n", payload.image.size);



    PacketSender packetSender(socket, {response_payload.data, response_payload.size});

    if(packetSender.Send())
    {
        printf("Server: Sent %zu bytes\n", response_payload.size);
    }
    else
    {
        printf("Server: Failed packet sender\n");
        return;
    }

    free(buffer);
    close(socket);
}
