#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <getopt.h>
#include <arpa/inet.h>
#include "../../utils/types.h"
#include "../ArgumentParser/ArgumentParser.h"
#include "../ImageHandler/ImageHandler.h"
#include "../../utils/types.h"
#include "../../utils/OSUtils.h"

class RequestHandler 
{
    int m_socket;
    struct sockaddr_in m_serv_addr;
    struct hostent* m_server;

    public:
    static RequestHandler& Instance();
    void SendRequest(Payload payload);
};




