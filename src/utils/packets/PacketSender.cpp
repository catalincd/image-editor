#include "PacketSender.h"

#include <thread>
#include <chrono>
using namespace std::chrono;

extern int COMMUNICATION_TIMEOUT;
extern int COMMUNICATION_TICK;
extern size_t MAX_PACKET;
extern size_t MAX_PAYLOAD;

const size_t MAX_PACKET_DATA = MAX_PACKET - sizeof(Packet);

PacketSender::PacketSender(int socket, const Buffer& data)
{
    m_socket = socket;
    m_buffer = data;
    m_lastSent = -1;
    m_session = 17; // change this to time based
    m_totalSent = 0;

    m_received.data = (char*)malloc(MAX_PAYLOAD);
    m_received.size = 0;
}


bool PacketSender::Send()
{
    //std::cout << "STARTED SENDING" << std::endl;
    bool sentAll = false;
    auto start = steady_clock::now();
    while(true)
    {
        auto now = steady_clock::now();
        auto elapsed = duration_cast<seconds>(now - start).count();
        if(elapsed > COMMUNICATION_TIMEOUT)
        {
            std::cerr << "COMMUNICATION_TIMEOUT" << std::endl;
            break;
        }

        if(m_buffer.size == m_totalSent)
        {
            SendBytes(SyncPacketToBytes({EOT, 0, m_session, m_buffer.size}));
            // std::cout << "SENT EOT" << std::endl;
            sentAll = true;
            break;
        }

        if(m_lastSent < 0)
        {
            // std::cout << "SENDING SOT" << std::endl;
            if(SendBytes(SyncPacketToBytes({SOT, 0, m_session, m_buffer.size})))
            {
                // std::cout << "SENT SOT" << std::endl;
                std::this_thread::sleep_for(milliseconds(5));


                // std::cout << "AWAITING ACK" << std::endl;
                if(ReceiveBytes(sizeof(SyncPacket)))
                {
                    // std::cout << "RECEIVED ACK / NOW VALIDATING" << std::endl;
                    SyncPacket receivedSync = BytesToSyncPacket(m_received);
                    if(receivedSync.code == ACK)
                    {
                        // std::cout << "VALIDATED ACK" << std::endl;
                        m_lastSent = 0;
                        continue;
                    }
                }
            }
        }
        else
        {
            


            size_t currentSize = m_buffer.size - (MAX_PACKET_DATA * m_lastSent);
            currentSize = currentSize < MAX_PACKET_DATA? currentSize : MAX_PACKET_DATA;

            //std::cout<<"TOTAL SIZE:\t"<<m_buffer.size<<"B"<<std::endl;
            //std::cout<<"SENT: \t\t"<<(MAX_PACKET_DATA * m_lastSent)<<"B"<<std::endl;
            //std::cout<<"SENDING: \t"<<currentSize<<"B"<<std::endl;
            
            if(SendBytes(PacketToBytes({m_lastSent, m_session, {m_buffer.data + (MAX_PACKET_DATA * m_lastSent), currentSize}})))
            {
                std::this_thread::sleep_for(milliseconds(5));

                //std::cout << "AWAITING ACK" << std::endl;
                if(ReceiveBytes(sizeof(SyncPacket)))
                {
                    //std::cout << "RECEIVED ACK / NOW VALIDATING" << std::endl;
                    SyncPacket receivedSync = BytesToSyncPacket(m_received);
                    if(receivedSync.code == ACK && receivedSync.packet_num == m_lastSent)
                    {
                        //std::cout << "VALIDATED ACK #" << m_lastSent << std::endl;
                        m_lastSent++;
                        m_totalSent += currentSize;
                        continue;
                    }
                    else if(receivedSync.code == EOT)
                    {
                        std::cerr << "EARLY EOT TRANSMISSION FAILED" << std::endl;
                        break;
                    }
                }
            }
        }

        std::this_thread::sleep_for(milliseconds(10));
    }
    return sentAll;
}


bool PacketSender::SendBytes(Buffer buffer)
{
    ssize_t total_sent = 0;
    auto start = steady_clock::now();
    while (total_sent < buffer.size)
    {
        auto now = steady_clock::now();
        auto elapsed = duration_cast<milliseconds>(now - start).count();
        if(elapsed > COMMUNICATION_TICK)
        {
            std::cerr << "COMMUNICATION_TICK_TIMEOUT" << std::endl;
            break;
        }
        ssize_t sent_size = send(m_socket, buffer.data + total_sent, buffer.size - total_sent, MSG_DONTWAIT);
        if (sent_size < 0)
        {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                std::this_thread::sleep_for(milliseconds(5));
                Error("ERROR write");
            }
            break;
        }
        else if (sent_size == 0)
        {
            break;
        }
        else
        {
            total_sent += sent_size;
            //std::cout<<"SENT "<<sent_size<<"B"<<std::endl;
        }
    }

    return (total_sent == buffer.size);
}

bool PacketSender::ReceiveBytes(size_t target = 0)
{

    ssize_t received_size;
    size_t total_received = 0;
    auto start = steady_clock::now();
    while (true)
    {
        auto now = steady_clock::now();
        auto elapsed = duration_cast<milliseconds>(now - start).count();
        if(elapsed > COMMUNICATION_TICK)
        {
            std::cerr << "COMMUNICATION_TICK_TIMEOUT" << std::endl;
            break;
        }
        received_size = read(m_socket, m_received.data + total_received, MAX_PAYLOAD - total_received);

        //std::cout<<"READ: "<<received_size<<"B"<<std::endl;

        if (received_size < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                // std::cerr << "RECEIVED -1" << std::endl;
                std::this_thread::sleep_for(milliseconds(10));
                continue;
            }
            else
            {
                Error("ERROR reading");
                close(m_socket);
                break;
            }
        }
        else if (received_size == 0)
        {
            break;
        }
        else
        {
            total_received += received_size;

            if(target > 0 && target <= total_received)
                break;
        }
    }

    m_received.size = total_received;

    return (total_received > 0);
}

PacketSender::~PacketSender()
{
    if(m_buffer.data != NULL) free(m_buffer.data);
    if(m_received.data != NULL) free(m_received.data);
}