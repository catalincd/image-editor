#include "OSUtils.h"



void Error(const char* error)
{
    perror(error);
    exit(1);
}


void Print(const char* str)
{
    std::cout << str << std::endl;
}


std::vector<std::string> Split(std::string str, char splitter)
{
    std::vector<std::string> container;
    
    size_t cursor;
    while ((cursor = str.find(splitter)) != std::string::npos)
    {
        std::string current = str.substr(0, cursor);
        if(current != "")
            container.push_back(current);
        str = str.substr(cursor + 1);
    }

    if (str != "")
        container.push_back(str);

    return container;
}

std::string GetFileExtension(const char* file)
{
    std::string str(file);
    return str.substr(str.find_last_of(".") - 1);
}

vector2 str_to_v2(const char* buffer)
{
    std::string str(buffer);
    auto words = Split(str, 'x');
    return { stoi(words[0]), stoi(words[1]) };
}

vector2 str_to_v2_offset(const char* buffer, const int offset = 0)
{
    std::string str(buffer);
    auto words = Split(str, 'x');
    return { stoi(words[0 + offset]), stoi(words[1 + offset]) };
}


extern size_t MAX_PAYLOAD;
Buffer PayloadToBytes(Payload payload) // append image data to payload struct bytes
{
    size_t struct_size = sizeof(Payload);
    size_t full_size = struct_size + payload.image.size;

    char* buffer = (char*)malloc(full_size);
    std::memcpy(buffer, &payload, struct_size);
    std::memcpy(buffer + struct_size, payload.image.data, payload.image.size);

    return { buffer, full_size };
}

std::string PayloadToString(Payload payload) 
{
    size_t struct_size = sizeof(Payload);
    size_t full_size = struct_size + payload.image.size;

    char* buffer = (char*)malloc(full_size + 1);
    std::memcpy(buffer, &payload, struct_size);
    std::memcpy(buffer + struct_size, payload.image.data, payload.image.size);

    buffer[full_size] = '\0';
    std::string str(buffer);
    free(buffer); //maybe not?

    return str;
}

std::string BytesToString(Buffer bytes)
{
    char* buffer = (char*)malloc(bytes.size + 1);
    std::memcpy(buffer, &bytes.data, bytes.size);
    buffer[bytes.size] = '\0';

    std::string str(buffer);
    free(buffer);
    
    return str;
}

Payload BytesToPayload(Buffer buffer)
{
    size_t struct_size = sizeof(Payload);
    Payload payload;
    std::memcpy(&payload, buffer.data, struct_size);

    size_t image_size = buffer.size - struct_size;
    char* image = buffer.data + struct_size;
    payload.image = {image, image_size};

    return payload;
}

extern size_t MAX_PACKET;

Buffer PacketToBytes(Packet packet) 
{
    size_t struct_size = sizeof(Packet);
    size_t full_size = struct_size + packet.data.size;
    
    // std::cout<<"PACKET TO BYTES: "<<packet.data.size<<"B /"<<full_size<<"B\n";

    char* buffer = (char*)malloc(full_size);
    std::memcpy(buffer, &packet, struct_size);
    std::memcpy(buffer + struct_size, packet.data.data, packet.data.size);

    return { buffer, full_size };
}

Packet BytesToPacket(Buffer buffer)
{
    size_t struct_size = sizeof(Packet);
    Packet packet;
    std::memcpy(&packet, buffer.data, struct_size);

    size_t data_size = buffer.size - struct_size;
    char* data = buffer.data + struct_size;
    packet.data = {data, data_size};

    return packet;
}


Buffer SyncPacketToBytes(SyncPacket ack)
{
    size_t struct_size = sizeof(SyncPacket);
    char* buffer = (char*)malloc(struct_size);
    std::memcpy(buffer, &ack, struct_size);
    return { buffer, struct_size };
}

SyncPacket BytesToSyncPacket(Buffer buffer)
{
    size_t struct_size = sizeof(SyncPacket);
    SyncPacket ack;
    std::memcpy(&ack, buffer.data, struct_size);
    return ack;
}

/*
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
*/
void PrintPayload(Payload payload)
{
    std::cout << "TARGET: \t\t" << payload.target << std::endl;
    std::cout << std::endl;


    std::cout << "RESIZE: \t\t" << payload.resize << std::endl;
    std::cout << "SIZE: \t\t(" << payload.size.x << "," << payload.size.y << ")" << std::endl;
    std::cout << std::endl;

    std::cout << "RECONTRAST: \t\t(" << payload.recontrast << ")" << std::endl;
    std::cout << "CONTRAST: \t\t" << payload.contrast << std::endl;
    std::cout << std::endl;

    std::cout << "RECONTRAST: \t\t(" << payload.rebright << ")" << std::endl;
    std::cout << "BRIGHTNESS: \t\t" << payload.brightness << std::endl;
    std::cout << std::endl;

    std::cout << "BLUR: \t\t(" << payload.blur << ")" << std::endl;
    std::cout << "KERNEL: \t\t" << payload.kernel << std::endl;
    std::cout << std::endl;

    std::cout << "ROTATE: \t\t(" << payload.rotate << ")" << std::endl;
    std::cout << "ANGLE: \t\t" << payload.angle << std::endl;
    std::cout << std::endl;

    std::cout << "CROP: \t\t" << payload.crop << std::endl;
    std::cout << "SIZE: \t\t(" << payload.cropTop.x << "," << payload.cropTop.y << ") x (" << payload.cropSize.x << "," << payload.cropSize.y << ")" << std::endl;
    std::cout << std::endl;
     
}