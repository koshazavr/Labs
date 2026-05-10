#include "CameraProvider.hpp"

CameraProvider::CameraProvider(int deviceID) {
    cap.open(deviceID);
}

CameraProvider::~CameraProvider() {
    if (cap.isOpened()) {
        cap.release();
    }
}

bool CameraProvider::isOpened() {
    return cap.isOpened();
}

bool CameraProvider::getFrame(cv::Mat& frame) {
    if (!cap.isOpened()) return false;
    return cap.read(frame);
}
