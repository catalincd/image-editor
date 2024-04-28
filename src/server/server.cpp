#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "SocketHandler/SocketHandler.h"

int main(int argc, char** argv)
{

    SocketHandler::Instance().SetupConnection();

    return 0;
}
