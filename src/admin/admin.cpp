#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "../utils/OSUtils.h"

int main()
{
    int unix_socket = 0;
    struct sockaddr_un server_addr;

    unix_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (unix_socket < 0)
        Error("ERROR socket unix");

    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, "/tmp/unix_socket", sizeof(server_addr.sun_path) - 1);

    if (connect(unix_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        Error("ERROR connect unix");
    }

    while(true)
    {
        int option = 0;
        printf("WELCOME ADMIN\nCHOOSE IF YOU WANT TO KILL THE SERVER OR NOT?:\n 1. KILL SERVER\n 2. DO NOT KILL THE SERVER(50/50)\n");
        std::cin >> option;
        if(option == 1)
            {
                const char* msg = "1";
                send(unix_socket, msg, strlen(msg), 0);
            }
        if(option == 2)
            {
                const char* msg = "3";
                send(unix_socket, msg, strlen(msg), 0);
            }
        
        char *buffer = (char *)malloc(1024);
        int check = read(unix_socket, buffer, 1024);
        if(check < 0)
            Error("ERROR read");

        printf("\n%s", buffer);

    }

    return 0;
}