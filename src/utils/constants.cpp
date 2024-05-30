#include <stdlib.h>

int PORT = 9090;
int COMMUNICATION_TIMEOUT = 5; //seconds
int COMMUNICATION_TICK = 50; //mseconds


size_t MAX_PACKET = 1024LL * 64LL;
size_t MAX_PAYLOAD = 1024LL * 1024LL * 100LL;
size_t MAX_IMAGE = 1024LL * 1024LL * 99LL;