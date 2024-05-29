#include "RequestHandler.h"

RequestHandler &RequestHandler::Instance()
{
    static RequestHandler instance;
    return instance;
}

extern size_t MAX_PAYLOAD;

void RequestHandler::SendRequest(Payload payload)
{
    Buffer bytes = PayloadToBytes(payload);

    if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        Error("Failed to create socket");

    m_serv_addr.sin_family = AF_INET;
    m_serv_addr.sin_port = htons(9090);                     // TO DO: add this as function param read from arguments
    inet_pton(AF_INET, "127.0.0.1", &m_serv_addr.sin_addr); // TO DO: add this as function param read from arguments

    if (connect(m_socket, (struct sockaddr *)&m_serv_addr, sizeof(m_serv_addr)) < 0)
        Error("Failed to connect");

    printf("Connected to %s:%d\n", "127.0.0.1", 9090);

    send(m_socket, bytes.data, bytes.size, 0);

    printf("Sent %zu bytes\n", bytes.size);

    char *received = (char *)malloc(MAX_PAYLOAD);
    size_t total_recieved = 0;
    ssize_t received_size;
    while (true)
    {
        received_size = read(m_socket, received + total_recieved, MAX_PAYLOAD - total_recieved);
        if (received_size < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                continue;
            }
            else
            {
                Error("ERROR reading");
                close(m_socket);
            }
        }
        else if (received_size == 0)
        {
            break;
        }
        else
        {
            total_recieved += received_size;
        }
    }
    printf("Recived %ld", total_recieved);

    Payload received_payload = BytesToPayload({received, total_recieved});

    ImageHandler::Instance().Write(received_payload.target, received_payload.image);
    printf("Written image\n");

    close(m_socket);
}