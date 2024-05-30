#include <thread>
#include "../../utils/types.h"
#include <vector>

class WebServer
{
    std::thread m_thread;
public:
    WebServer();

    void Start();
    void Join();

    static std::vector<char> DecodeByte64(const std::string&);
};