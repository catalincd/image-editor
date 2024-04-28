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

class ArgumentParser 
{
    char* outfile;

    public:
    static ArgumentParser& Instance();
    Payload HandleArguments(int argc, char** argv);
    char* GetOutputFile(); 
};




