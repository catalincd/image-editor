#include "Filters.h"

using namespace cv;

Filters& Filters::Instance()
{
    static Filters instance;
    return instance;
}


Mat Filters::Resize(Mat input, vector2 size)
{
    Mat output;
    resize(input, output, Size(size.x, size.y), INTER_LINEAR);
    return output;
}

Mat Filters::Rebright(Mat input, float brightness)
{
    Mat output, hsv, adjustedHSV;
    cvtColor(input, hsv, COLOR_BGR2HSV);
    std::vector<Mat> channels;
    split(hsv, channels);
    channels[2] += -50 + brightness;
    merge(channels, adjustedHSV);
    cvtColor(adjustedHSV, output, COLOR_HSV2BGR);

    return output;
}