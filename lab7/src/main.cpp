#include "CameraProvider.hpp"
#include "Display.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "FaceDetector.hpp"
#include <iostream>

int main() {
    CameraProvider camera(0);
    Display display("Lab 7");
    KeyProcessor keyHandler;
    FrameProcessor frameProcessor;
    
    FaceDetector faceDetector("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");

    MouseData mData;
    int brightness = 50;
    display.setupMouse(&mData);
    display.setupTrackbar(&brightness, 100);

    if (!camera.isOpened()) return -1;

    cv::Mat frame;
    double tick = 0;
    bool faceDetectionEnabled = false;

    while (true) {
        double prevTick = tick;
        tick = (double)cv::getTickCount();
        double fps = cv::getTickFrequency() / (tick - prevTick);

        if (camera.getFrame(frame)) {
            frame.convertTo(frame, -1, 1, brightness - 50);

            int key = cv::waitKey(1);
            if (key == 27) break;
            
            if (key == 'f' || key == 'F') {
                faceDetectionEnabled = !faceDetectionEnabled;
            }
            keyHandler.handleKey(key);

            frameProcessor.process(frame, keyHandler.getMode());

            if (faceDetectionEnabled) {
                faceDetector.updateFrame(frame);
                
                auto faces = faceDetector.getFaces();
                for (const auto& rect : faces) {
                    cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 3);
                    cv::putText(frame, "Face Detected", cv::Point(rect.x, rect.y - 10), 
                                cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
                }
            }

            if (keyHandler.getMode() == Mode::DRAW && (mData.drawing || mData.readyToDraw)) {
                cv::rectangle(frame, mData.start, mData.end, cv::Scalar(0, 0, 255), 2);
            }

            display.drawOverlay(frame, keyHandler.getModeName(), cv::Point(10, 30));
            display.drawOverlay(frame, "FPS: " + std::to_string((int)fps), cv::Point(10, 60));
            display.drawOverlay(frame, "Press 'F' to toggle Face Detection", cv::Point(10, frame.rows - 20));

            display.show(frame);
        }
    }
    return 0;
}
