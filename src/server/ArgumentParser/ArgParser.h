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
#include "../../utils/types.h"      
#include "../ImageHandler/ImageHandler.h"
#include "../../utils/types.h" 
#include "../../utils/OSUtils.h" 


struct Payload {
    std::string ip;
    int port;
    int threads;
    int max_clients;
};

class ArgumentParserServer {
private:
    std::string outfile;
    ArgumentParserServer() = default;
    
public:
    static ArgumentParserServer& Instance();
    Payload HandleArguments(int argc, char** argv);
    const std::string& GetOutputFile() const;
    static struct option long_options[];
};




