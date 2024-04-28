#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>
#include <getopt.h>
#include "../utils/types.h"
#include "ArgumentParser/ArgumentParser.h"
#include "RequestHandler/RequestHandler.h"


int main(int argc, char** argv)
{
    Payload payload = ArgumentParser::Instance().HandleArguments(argc, argv);
    RequestHandler::Instance().SendRequest(payload);

    return 0;
}
