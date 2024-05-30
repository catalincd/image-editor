#include "PacketReceiver.h"

#include <thread>
#include <chrono>
using namespace std::chrono;

extern int COMMUNICATION_TIMEOUT;
extern int COMMUNICATION_TICK;
extern size_t MAX_PACKET;
extern size_t MAX_PAYLOAD;

const size_t MAX_PACKET_DATA = MAX_PACKET - sizeof(Packet);

PacketReceiver::PacketReceiver(int socket)
{
    m_socket = socket;
    m_buffer.data = (char*)malloc(MAX_PAYLOAD);
    m_buffer.size = 0;

    m_receivedSize = 0;
    m_received.data = (char*)malloc(MAX_PAYLOAD);
    m_received.size = 0;
}

Buffer& PacketReceiver::GetBuffer()
{
    return m_buffer;
}

bool PacketReceiver::Receive()
{
    // std::cout << "STARTED RECEIVING" << std::endl;

    bool sentAll = false;
    m_state = IDLE;
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

        //std::cerr << "RECV COUNT: " << m_receivedSize << std::endl;

        

        if(m_buffer.size != 0 && m_receivedSize >= m_buffer.size)
        {
            m_state = FULL;
        }

        if(m_state == IDLE)
        {
            if(ReceiveBytes(sizeof(SyncPacket)))
            {
                // std::cout << "RECEIVED SOT" << std::endl;
                SyncPacket receivedSync = BytesToSyncPacket(m_received);
                if(receivedSync.code == SOT)
                {
                    if(SendBytes(SyncPacketToBytes({ACK, 0, m_session, m_buffer.size})))
                    {
                        // std::cout << "VALIDATED SOT" << std::endl;
                        m_state = RECEIVING;
                        m_receivedCount = 0;
                        m_buffer.size = receivedSync.total_data;
                        m_session = receivedSync.session;
                    }
                }
            }
            else // CONTINENTAL STYLE STATEMENT
            {
                //std::cout << "AWAITING SOT" << std::endl;
            }
        } 
        else if(m_state == RECEIVING)
        {
            size_t maxLen = m_buffer.size - m_receivedSize;
            maxLen = maxLen > MAX_PACKET? MAX_PACKET : maxLen;

            if(ReceiveBytes(maxLen))
            {
                
                Packet receivedPacket = BytesToPacket(m_received);
                //std::cout << "RECEIVED Packet " << receivedPacket.data.size << "B" << std::endl;

                if(receivedPacket.data.size + sizeof(Packet) != m_received.size)
                {
                    std::cout << "BROKEN PACKET" << std::endl;
                    break;
                }

                if(receivedPacket.session == m_session)
                {
                    if(receivedPacket.packet_num > m_receivedCount)
                    {
                        std::cerr << "PACKET INCREMENT MISMATCH" << std::endl;
                        break;
                    }

                    if(receivedPacket.packet_num < m_receivedCount)
                    {
                        SendBytes(SyncPacketToBytes({ACK, receivedPacket.packet_num, m_session, m_buffer.size}));
                        continue;
                    }
                    
                    memcpy(m_buffer.data + (m_receivedCount * MAX_PACKET_DATA), receivedPacket.data.data, receivedPacket.data.size);
                    m_receivedSize += receivedPacket.data.size;
                    
                    //std::cout << "SENDING ACK" << std::endl;
                    if(SendBytes(SyncPacketToBytes({ACK, m_receivedCount, m_session, m_buffer.size})))
                    {
                        //std::cout << "SENT ACK #" << m_receivedCount << std::endl;
                        m_receivedCount++;
                    }
                }
                else
                {
                    std::cout << "SESSION MISMATCH" << std::endl;
                }

                //if(receivedPacket.data.data != NULL)free(receivedPacket.data.data);
            }
        }
        if(m_state == FULL)
        {
            if(ReceiveBytes(sizeof(SyncPacket)))
            {
                //std::cout << "RECEIVED SyncPacket" << std::endl;
                SyncPacket receivedSync = BytesToSyncPacket(m_received);
                if(receivedSync.code == EOT)
                {
                    m_state = ENDED;
                    sentAll = (m_receivedSize == m_buffer.size);
                    break;
                }
            }
            else // ANOTHER CONTINENTAL STYLE STATEMENT
            {
                //std::cerr << "AWAITING EOT" << std::endl;
            }
        }

        std::this_thread::sleep_for(milliseconds(10));
    }
    return sentAll;
}

bool PacketReceiver::SendBytes(Buffer buffer)
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

bool PacketReceiver::ReceiveBytes(size_t target = 0)
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
                //std::cerr << "RECEIVED -1" << std::endl;
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
    //std::cout<<"END READ"<<std::endl;

    return (total_received > 0);
}

PacketReceiver::~PacketReceiver()
{
    if(m_buffer.data != NULL) free(m_buffer.data);
    if(m_received.data != NULL) free(m_received.data);
}