#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "SocketHandler/SocketHandler.h"
#include "ArgumentParser/ArgParser.h"
#include "WebServer/WebServer.h"

int main(int argc, char** argv)
{
    ArgumentParserServer& parser = ArgumentParserServer::Instance();

    PayloadServer config = parser.HandleArguments(argc, argv);

    WebServer webServer = WebServer();

    std::cout << "Server:\n"
              << "IP: " << config.ip << "\n"
              << "Port: " << config.port << "\n"
              << "Threads: " << config.threads << "\n"
              << "Max Clients: " << config.max_clients << std::endl;

    SocketHandler::Instance().SetupConnection();

    webServer.Join();

    return 0;
}
