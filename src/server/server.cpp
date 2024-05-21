#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "SocketHandler/SocketHandler.h"
#include "ArgParser.h"

int main(int argc, char** argv)
{
    ArgumentParserServer& parser = ArgumentParserServer::Instance();

    Payload config = parser.HandleArguments(argc, argv);


    std::cout << "Server:\n"
              << "IP: " << config.ip << "\n"
              << "Port: " << config.port << "\n"
              << "Threads: " << config.threads << "\n"
              << "Max Clients: " << config.max_clients << std::endl;

    SocketHandler::Instance().SetupConnection();

    return 0;
}
