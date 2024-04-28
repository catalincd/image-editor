#pragma once

class ConnectionHandler 
{
    public:
    static ConnectionHandler& Instance();
    static void HandleConnection(int socket);
};




