#pragma once

#include <opencv2/opencv.hpp>
#include "../../utils/types.h"

class Filters
{
public:
    static Filters& Instance();
    cv::Mat Resize(cv::Mat input, vector2 size);
    cv::Mat Recontrast(cv::Mat input, float contrast);
    cv::Mat Rebright(cv::Mat input, float brightness);
    cv::Mat Blur(cv::Mat input, float kernel);
    cv::Mat Rotate(cv::Mat input, float angle);
    cv::Mat Crop(cv::Mat input, vector2 top, vector2 size);
};