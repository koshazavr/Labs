#ifndef CAMERA_PROVIDER_HPP
#define CAMERA_PROVIDER_HPP

#include <opencv2/opencv.hpp>

class CameraProvider {
private:
    cv::VideoCapture cap;

public:
    CameraProvider(int deviceID = 0);
    
    ~CameraProvider();

    bool isOpened();

    bool getFrame(cv::Mat& frame);
};

#endif
