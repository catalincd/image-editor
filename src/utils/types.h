#pragma once

struct vector2 {
    int x = 0;
    int y = 0;
};

struct Buffer {         // maybe add on destroy (eventually)
    char* data = NULL;
    size_t size;
};

// TO DO: modify this, add more stuff to it or something
struct Payload {
    char target[255];

    bool resize = false;
    vector2 size;
    
    bool recontrast = false;
    float contrast = 0.0f;

    bool rebright = false;
    float brightness = 0.0f;

    bool blur = false;
    float kernel = 0.0f;

    bool rotate = false;
    float angle = 0.0f;

    bool crop = false;
    vector2 cropTop = {0, 0};
    vector2 cropSize = {0, 0};

    Buffer image;
};

struct Packet { 
    int packet_num;
    int session;
    Buffer data;
};

enum SyncCode { EOT, SOT, ACK} ;

struct SyncPacket {
    SyncCode code;
    int packet_num;
    int session;
    size_t total_data;
};
