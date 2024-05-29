#include "PacketSender.h"


extern size_t MAX_PACKET;
extern size_t MAX_PAYLOAD;
extern size_t PACKET_HEADER;

extern int COMMUNICATION_TIMEOUT;
extern int COMMUNICATION_TICK;

const size_t MAX_PACKET_DATA = MAX_PACKET - sizeof(Packet);

using namespace std::chrono;


PacketSender::PacketSender(int socket, Buffer buffer) 
{
    std::cout << "STRUCT PACKET: " << sizeof(Packet) << "B" << std::endl;
    std::cout << "MAX PACKET: " << MAX_PACKET << "B"  << std::endl;
    std::cout << "MAX PACKET DATA: " << MAX_PACKET_DATA << "B"  << std::endl;

    m_socket = socket;
    m_buffer = buffer;

    m_packets_num = buffer.size / MAX_PACKET_DATA + 1;
    m_sent_count = 0;
    m_total_sent_count = 0;
    m_session = 17;

    m_packets.resize(m_packets_num);
    m_ack.resize(m_packets_num);
}

bool PacketSender::Send()
{
    int REMAINING_BYTES = m_buffer.size;
    for(int i=0;i<m_packets_num;i++)
    {
        m_packets[i].packet_num = i;
        m_packets[i].total_num = m_packets_num;
        m_packets[i].session = m_session;
        m_packets[i].total_data = m_buffer.size;


        int packetSize = REMAINING_BYTES < MAX_PACKET_DATA? REMAINING_BYTES : MAX_PACKET_DATA;
        REMAINING_BYTES -= packetSize;

        m_packets[i].data.size = packetSize;
        m_packets[i].data.data = (char*)malloc(packetSize);
        std::memcpy(m_packets[i].data.data, m_buffer.data + (MAX_PACKET_DATA * i), packetSize);

        m_ack[i] = false;
    }

    m_thread = std::thread(&PacketSender::StartAccepting, this);
    auto start = steady_clock::now();

    std::cout<<"SENDING "<<m_packets_num<<" PACKETS"<<std::endl;

    while(m_sent_count < m_packets_num)
    {   
        std::cout<<"SENDING PACKETS " << m_sent_count <<" / " << m_packets_num << std::endl;
        auto now = steady_clock::now();
        auto elapsed = duration_cast<seconds>(now - start).count();
        if(elapsed > COMMUNICATION_TIMEOUT)
        {
            break;
        }

        for(int i=0;i<m_packets_num;i++)
        {
            m_lock.lock();

            if(!m_ack[i])
            {
                Buffer packetBuffer = PacketToBytes(m_packets[i]);

                std::cout<< "PACKET #" << i << " SIZE: " << packetBuffer.size << "B" << std::endl;

                send(m_socket, packetBuffer.data, packetBuffer.size, 0);

                m_total_sent_count++;

                free(packetBuffer.data);
            }
                

            m_lock.unlock();
        }

        std::this_thread::sleep_for(milliseconds(COMMUNICATION_TICK));
    }
    
    m_thread.join();

    for(int i=0;i<m_packets_num;i++)
    {
        free(m_packets[i].data.data);
    }

    std::cout << "ENDED TRANSIMISSION " << std::endl;
    std::cout << "LOSS: " << (float(m_total_sent_count - m_packets_num) / float(m_total_sent_count)) * 100.0f << "%" << std::endl;

    return (m_sent_count == m_packets_num);
}

void printByteArrayX(char *array, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("0x%02X ", array[i]);
    }
    printf("\n");
}

void PacketSender::StartAccepting()
{
    std::cout<<"Started accepting"<<std::endl;

    auto start = steady_clock::now();
    while(m_sent_count < m_packets_num)
    {
        auto now = steady_clock::now();
        auto elapsed = duration_cast<seconds>(now - start).count();
        if(elapsed > COMMUNICATION_TIMEOUT)
        {
            break;
        }
        try
        {
            char* received = (char*)malloc(MAX_PACKET);
            ssize_t received_size = read(m_socket, received, MAX_PACKET);

            if(received_size < sizeof(Ack) || received_size % sizeof(Ack) != 0)
            {
                if(received_size != 0)
                    std::cout<<"RECEIVED WRONG ACK: "<< received_size << "B != " << sizeof(Ack) << "B" <<std::endl;
                
                continue;
            }

            for(int cursor = 0; cursor < received_size; cursor += sizeof(Ack))
            {
                Ack ack = BytesToAck({received + cursor, sizeof(Ack)});
                std::cout<<"RECEIVED ACK "<<ack.packet_num<<"/"<<ack.total_num<<std::endl;
                printByteArrayX(received, received_size);
                
                if(ack.session == m_session)
                {
                    m_lock.lock();
                    m_ack[ack.packet_num] = true;
                    m_sent_count += 1;
                    m_lock.unlock();
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        // std::this_thread::sleep_for(milliseconds(COMMUNICATION_TICK));
    }

    std::cout<<"Ended accepting"<<std::endl;
}

int PacketSender::GetSocket()
{
    return m_socket;
}



PacketSender::~PacketSender()
{
    // try to free mem and join thread again?
}