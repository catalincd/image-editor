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