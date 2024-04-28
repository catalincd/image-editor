#include "ImageHandler.h"

extern size_t MAX_IMAGE;

ImageHandler& ImageHandler::Instance()
{
    static ImageHandler instance;
    return instance;
}




Buffer ImageHandler::Read(const char* path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) Error("Failed to open file");

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    char* image = (char*)malloc(fileSize);
    file.read(image, fileSize);

    if (!file) Error("Failed to read file");

    file.close();

    return { image, fileSize };
}


void ImageHandler::Write(const char* path, Buffer image)
{
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) Error("Failed to open file");
    file.write(image.data, image.size);
    if (!file) Error("Failed to write to file");
    file.close();
}