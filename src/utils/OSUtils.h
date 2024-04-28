#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "types.h"

void Error(const char*);
void Print(const char*);
std::vector<std::string> Split(std::string, char);
std::string GetFileExtension(const char*);
vector2 str_to_v2(const char*);

Payload BytesToPayload(Buffer);
Buffer PayloadToBytes(Payload);