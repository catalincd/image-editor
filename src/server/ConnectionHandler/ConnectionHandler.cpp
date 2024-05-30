#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

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

void ConnectionHandler::HandleAdminConnection(int socket)
{
    struct sockaddr_un unix_addr;
    socklen_t addr_len = sizeof(unix_addr);

    int unix_con = accept(socket, (struct sockaddr*) &unix_addr, &addr_len);
    if (unix_con < 0)
        Error("ERROR on accept");

    char *buffer = (char *)malloc(MAX_PAYLOAD);
    ssize_t buffer_size = read(unix_con, buffer, MAX_PAYLOAD);
    if (buffer_size < 0)
        Error("ERROR on read");
    
    if (strcmp(buffer, "1") == 0)
    {
        const char *response = "The server will be closed";
        send(unix_con, response, strlen(response), 0);
        printf("THE ADMIN KILLED THE SERVER ALEGIDLYDLY I HAVAE DYSLEXIA");
        exit(0);
    }
    else
    {
        const char *response = "Have a nice day :)";
        send(unix_con, response, strlen(response), 0);
    }

    close(unix_con);
}