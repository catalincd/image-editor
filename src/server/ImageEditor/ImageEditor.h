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
#include "../../utils/OSUtils.h"

#include "../Filters/Filters.h"

class ImageEditor 
{
    public:
    static ImageEditor& Instance();
    Buffer Edit(Payload payload);
};




