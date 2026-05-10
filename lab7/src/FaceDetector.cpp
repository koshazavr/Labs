#include "FaceDetector.hpp"

FaceDetector::FaceDetector(const std::string& proto, const std::string& model) : running(true) {
    net = cv::dnn::readNetFromCaffe(proto, model);
    
    worker = std::thread(&FaceDetector::detectLoop, this);
}

FaceDetector::~FaceDetector() {
    running = false;
    if (worker.joinable()) {
        worker.join();
    }
}

void FaceDetector::updateFrame(const cv::Mat& frame) {
    std::lock_guard<std::mutex> lock(frameMutex);
    frame.copyTo(currentFrame);
}

std::vector<cv::Rect> FaceDetector::getFaces() {
    std::lock_guard<std::mutex> lock(frameMutex);
    return faces;
}

void FaceDetector::detectLoop() {
    while (running) {
        cv::Mat frameCopy;
        
        {
            std::lock_guard<std::mutex> lock(frameMutex);
            if (currentFrame.empty()) continue;
            currentFrame.copyTo(frameCopy);
        }

        cv::Mat blob = cv::dnn::blobFromImage(frameCopy, 1.0, cv::Size(300, 300), 
                                              cv::Scalar(104.0, 177.0, 123.0));
        net.setInput(blob);
        cv::Mat detections = net.forward();

        std::vector<cv::Rect> detectedFaces;
        float* data = (float*)detections.data;

        for (int i = 0; i < detections.size[2]; i++) {
            float confidence = data[i * 7 + 2];
            if (confidence > 0.5) {
                int x1 = static_cast<int>(data[i * 7 + 3] * frameCopy.cols);
                int y1 = static_cast<int>(data[i * 7 + 4] * frameCopy.rows);
                int x2 = static_cast<int>(data[i * 7 + 5] * frameCopy.cols);
                int y2 = static_cast<int>(data[i * 7 + 6] * frameCopy.rows);
                detectedFaces.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
            }
        }

        {
            std::lock_guard<std::mutex> lock(frameMutex);
            faces = detectedFaces;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
