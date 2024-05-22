#include "ImageEditor.h"

using namespace cv;

ImageEditor &ImageEditor::Instance()
{
    static ImageEditor instance;
    return instance;
}

extern size_t MAX_IMAGE;

Buffer ImageEditor::Edit(Payload payload)
{
    Mat image = imdecode(Mat(1, payload.image.size, CV_8UC1, payload.image.data), IMREAD_UNCHANGED);

    if (payload.resize)
        image = Filters::Instance().Resize(image, payload.size);
    if (payload.rebright)
        image = Filters::Instance().Rebright(image, payload.brightness);

    std::vector<unsigned char> buffer(MAX_IMAGE);
    imencode(GetFileExtension(payload.target), image, buffer);

    size_t imageSize = buffer.size();
    char *imageData = (char *)malloc(imageSize); // this is still unsigned...
    memcpy(imageData, buffer.data(), imageSize);

    return {imageData, imageSize};
}
