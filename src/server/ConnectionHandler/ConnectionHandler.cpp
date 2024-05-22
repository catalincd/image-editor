#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ConnectionHandler.h"
#include "../ImageEditor/ImageEditor.h"
#include "../../utils/OSUtils.h"

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
    char *buffer = (char *)malloc(MAX_PAYLOAD);
    size_t buffer_size = read(socket, buffer, MAX_PAYLOAD);
    if (buffer_size < 0)
        Error("ERROR reading from socket");

    printf("Server: Received %d bytes\n", buffer_size);
    // printf("Str: \"%.16s\"\n", buffer);
    // printf("First 16: \"%.16hx\"\n", buffer);
    // SwapBytes(buffer, buffer_size);
    // printf("Swapped 16: \"%.16hx\"\n", buffer);

    //
    Payload payload = BytesToPayload({buffer, buffer_size});
    payload.image = ImageEditor::Instance().Edit(payload); // delete old image so we don't leak mem? nah overrated
    Buffer response_payload = PayloadToBytes(payload);
    // TODO: HANDLE BIZONIC ERRORS
    printf("Server: Edited image %d bytes\n", payload.image.size);

    size_t sent_size = write(socket, response_payload.data, response_payload.size);
    if (sent_size < 0)
        Error("ERROR writing to socket");
    printf("Server: Sent %d bytes\n", sent_size);

    close(socket);
}
