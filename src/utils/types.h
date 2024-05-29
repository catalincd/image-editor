#pragma once

struct vector2 {
    int x = 0;
    int y = 0;
};

struct Buffer {         // maybe add on destroy (eventually)
    char* data;
    size_t size;
};


struct Packet { 
    int packet_num;
    int total_num;
    int session;
    int total_data;
    Buffer data;
};

struct Ack {
    int packet_num;
    int total_num;
    int session;
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

    Buffer image;
};