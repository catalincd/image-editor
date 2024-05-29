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

    // send(m_socket, bytes.data, bytes.size, 0);
    PacketSender packetSender(m_socket, bytes);

    if(packetSender.Send())
    {
        printf("Sent %zu bytes\n", bytes.size);
    }
    else
    {
        printf("Failed packet sender\n");
        return;
    }


    PacketReceiver packetReceiver(m_socket);
    Buffer received_buffer;

    if(packetReceiver.Receive())
    {
        received_buffer = packetReceiver.GetBuffer();
        printf("Received %d bytes\n", received_buffer.size);
    }
    else
    {
        printf("Failed receiver");
        return;
    }

    Payload received_payload = BytesToPayload(received_buffer);

    ImageHandler::Instance().Write(received_payload.target, received_payload.image);
    printf("Written image\n");

    close(m_socket);
}