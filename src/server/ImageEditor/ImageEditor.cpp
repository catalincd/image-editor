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
    if (payload.recontrast)
        image = Filters::Instance().Recontrast(image, payload.contrast);
    if (payload.blur)
        image = Filters::Instance().Blur(image, payload.kernel);
    if (payload.rotate)
        image = Filters::Instance().Rotate(image, payload.angle);
    if (payload.crop)
        image = Filters::Instance().Crop(image, payload.cropTop, payload.cropSize);

    std::vector<unsigned char> buffer(MAX_IMAGE);
    imencode(GetFileExtension(payload.target), image, buffer);

    size_t imageSize = buffer.size();
    char *imageData = (char *)malloc(imageSize); // this is still unsigned...
    memcpy(imageData, buffer.data(), imageSize);

    return {imageData, imageSize};
}
