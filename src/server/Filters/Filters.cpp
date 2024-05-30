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

Mat Filters::Recontrast(Mat input, float contrast)
{
    std::cout<<"CONTRAST: "<<contrast<<std::endl;
    cv::Mat dst;
    input.convertTo(dst, -1, contrast, 50);
    return dst;
}

Mat Filters::Blur(cv::Mat src, float kernel)
{
    cv::Mat dst;
    double sigmaX = 0;
    int qKernel = int(kernel / 2) * 2 + 1;
    cv::GaussianBlur(src, dst, cv::Size(qKernel, qKernel), sigmaX);
    return dst;
}

Mat Filters::Crop(cv::Mat src, vector2 top, vector2 size)
{
    cv::Rect roi(top.x, top.y, size.x, size.y);
    cv::Mat croppedImage = src(roi);
    return croppedImage;
}



Mat Filters::Rotate(cv::Mat src, float angle)
{
    double scale = 1.0;

    cv::Point2f center(src.cols / 2.0, src.rows / 2.0);
    cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, scale);

    cv::Rect bbox = cv::RotatedRect(center, src.size(), angle).boundingRect();

    rotMat.at<double>(0, 2) += bbox.width / 2.0 - center.x;
    rotMat.at<double>(1, 2) += bbox.height / 2.0 - center.y;

    cv::Mat dst;
    cv::warpAffine(src, dst, rotMat, bbox.size());
    return dst;
}