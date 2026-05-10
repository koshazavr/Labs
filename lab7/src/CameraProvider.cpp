#include "CameraProvider.hpp"
#include <iostream>

CameraProvider::CameraProvider(int deviceID) {
    cap.open(deviceID, cv::CAP_V4L2);
    
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        cap.set(cv::CAP_PROP_FPS, 30);
        cap.set(cv::CAP_PROP_BUFFERSIZE, 1);

        std::cout << "[Camera] USB 3.1 MJPEG 640x480 Initialized" << std::endl;
    }
}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) cap.release();
}

bool CameraProvider::isOpened() { return cap.isOpened(); }

bool CameraProvider::getFrame(cv::Mat& frame) {
    if (!cap.isOpened()) return false;
    cap.grab(); 
    return cap.retrieve(frame);
}
