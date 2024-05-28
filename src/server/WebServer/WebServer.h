#include <thread>

class WebServer
{
    std::thread m_thread;
public:
    WebServer();

    void Start();
    void Join();
};