#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <getopt.h>
#include "../../utils/types.h"
#include "../../utils/OSUtils.h"

class ImageHandler 
{
    public:
    static ImageHandler& Instance();
    Buffer Read(const char* path);
    void Write(const char* path, Buffer data);
    
};