#pragma once

#include <opencv2/opencv.hpp>
#include "../../utils/types.h"

class Filters
{
public:
    static Filters& Instance();
    cv::Mat Resize(cv::Mat input, vector2 size);
    cv::Mat Rebright(cv::Mat input, float brightness);
};