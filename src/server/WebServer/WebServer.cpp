#include "WebServer.h"
#include <crow.h>
#include <fstream>  

#define CROW_STATIC_DIRECTORY "static/"
#define CROW_STATIC_ENDPOINT "/"

std::ofstream logfile("file.out");

class CrowLogger : public crow::ILogHandler
{
public:
    CrowLogger() {}
    void log(std::string message, crow::LogLevel /*level*/)
    {
        logfile << message << std::endl;
    }

    ~CrowLogger() {
        logfile.close();
    }
};

WebServer::WebServer() : m_thread(&WebServer::Start, this) {}

void WebServer::Join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void WebServer::Start()
{
    std::cout << "RUNNING CROW SERVER" << std::endl;

    const char *url = "http://0.0.0.0:18080/static/index.html";
    std::string command = "xdg-open " + std::string(url);
    //system(command.c_str());

    CrowLogger logger;
    crow::logger::setHandler(&logger);

    crow::SimpleApp app;
    app.port(18080).multithreaded().run();
    std::cout << "RAN CROW SERVER" << std::endl;
}
