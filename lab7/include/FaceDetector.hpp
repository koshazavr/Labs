#ifndef FACE_DETECTOR_HPP
#define FACE_DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

class FaceDetector {
private:
    cv::dnn::Net net;
    std::thread worker;
    std::mutex frameMutex;
    std::atomic<bool> running;
    
    cv::Mat currentFrame;
    std::vector<cv::Rect> faces;

    void detectLoop();

public:
    FaceDetector(const std::string& proto, const std::string& model);
    ~FaceDetector();

    void updateFrame(const cv::Mat& frame);
    std::vector<cv::Rect> getFaces();
};

#endif
