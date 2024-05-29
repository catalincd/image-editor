#include "PacketReceiver.h"

using namespace std::chrono;

extern size_t MAX_PACKET;
extern size_t MAX_PAYLOAD;
extern size_t PACKET_HEADER;

extern int COMMUNICATION_TIMEOUT;
extern int COMMUNICATION_TICK;

const size_t MAX_PACKET_DATA = MAX_PACKET - sizeof(Packet);

PacketReceiver::PacketReceiver(int socket)
{
    m_socket = socket;
    m_init = false;
    m_received_count = -1;
    m_packets_num = 1;
}

void printByteArray(char *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("0x%02X ", array[i]);
    }
    printf("\n");
}

bool PacketReceiver::Receive()
{
    auto start = steady_clock::now();

    while(m_received_count < m_packets_num)
    {
        std::this_thread::sleep_for(milliseconds(20));
        std::cout<<"RECEIVER LOOP " << m_received_count << "/" << m_packets_num <<std::endl;
        auto now = steady_clock::now();
        auto elapsed = duration_cast<seconds>(now - start).count();
        if(elapsed > COMMUNICATION_TIMEOUT)
        {
            break;
        }

        char* received = (char*)malloc(MAX_PACKET);
        size_t received_size = read(m_socket, received, MAX_PACKET);
        Packet packet = BytesToPacket({received, received_size});

        if(!m_init) Init(packet);

        if(packet.total_num != m_packets_num || packet.session != m_session)
        {
            std::cout<<"WRONG PACKET #" << packet.session << " " << packet.packet_num << "/" << packet.total_num <<std::endl;
            continue;
        }

        std::cout<<"RECEIVER: " << received_size << "B" <<std::endl;
        std::cout<<"PACKET: " << packet.data.size << "B " << packet.packet_num << "/" << packet.total_num << " TOTAL: " << packet.total_data << std::endl;
        
        //printByteArray(received, received_size);

        if(!m_ack[packet.packet_num])
        {
            m_ack[packet.packet_num] = true;
            int offset = MAX_PACKET_DATA * packet.packet_num;
            memcpy(m_buffer.data + offset, packet.data.data, packet.data.size);     
            m_received_count++;
            SendAck(packet);
        }

        std::cout<<"COUNTER: " << m_received_count << "/" << m_packets_num <<std::endl;

        free(received);
        //free(packet.data.data);
    }

    return (m_received_count == m_packets_num); 
}

Buffer& PacketReceiver::GetBuffer()
{
    return m_buffer;
}

void PacketReceiver::SendAck(Packet packet)
{
    Buffer ackData = AckToBytes({packet.packet_num, packet.total_num, packet.session});
    send(m_socket, ackData.data, ackData.size, 0);
    free(ackData.data);
}

void PacketReceiver::Init(Packet packet)
{
    m_init = true;
    m_received_count = 0;
    m_packets_num = packet.total_num;
    m_session = packet.session;


    m_buffer.size = packet.total_data;
    m_buffer.data = (char*)malloc(packet.total_data);

    m_ack.resize(m_packets_num);
    for(int i=0;i<m_packets_num;i++)
    {
        m_ack[i] = false;
    }
}

PacketReceiver::~PacketReceiver()
{
    free(m_buffer.data);
}