#include "RequestHandler.h"

#include "../../utils/packets/PacketSender.h"
#include "../../utils/packets/PacketReceiver.h"

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
    inet_pton(AF_INET, strlen(payload.server) > 0? payload.server : "127.0.0.1", &m_serv_addr.sin_addr); // TO DO: add this as function param read from arguments

    if (connect(m_socket, (struct sockaddr *)&m_serv_addr, sizeof(m_serv_addr)) < 0)
        Error("Failed to connect");

    printf("Connected to %s:%d\n", "127.0.0.1", 9090);

    
    PacketSender packetSender(m_socket, bytes);

    if(packetSender.Send())
    {
        std::cout << "Sent " << bytes.size << "B" << std::endl;
    }
    else
    {
        std::cerr << "FAILED PacketSender" << std::endl;
        return;
    }

    Payload received_payload;
    PacketReceiver packetReceiver(m_socket);


    if(packetReceiver.Receive())
    {
        Buffer receivedBuffer = packetReceiver.GetBuffer();
        received_payload = BytesToPayload(receivedBuffer);
        std::cout << "Received " << receivedBuffer.size << "B" << std::endl;
    }
    else
    {
        std::cerr << "FAILED packetReceiver" << std::endl;
    }



    ImageHandler::Instance().Write(received_payload.target, received_payload.image);
    printf("Written image\n");

    close(m_socket);
}