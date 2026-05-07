#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "CameraProvider.hpp"
#include "Display.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"

int main() {
    CameraProvider camera(0);
    Display display("Lab 6");
    KeyProcessor keyHandler;
    FrameProcessor frameProcessor;

    MouseData mData;
    int brightness = 50;
    
    display.setupMouse(&mData);
    display.setupTrackbar(&brightness, 100);

    if (!camera.isOpened()) {
        std::cerr << "CRITICAL ERROR: Camera not found!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    double tick = 0;

    while (true) {
        double prevTick = tick;
        tick = (double)cv::getTickCount();
        double fps = cv::getTickFrequency() / (tick - prevTick);

        if (camera.getFrame(frame)) {
            
            frame.convertTo(frame, -1, 1, brightness - 50);

            int key = cv::waitKey(1); 
            if (key == 27) break; // Вихід на ESC
            keyHandler.handleKey(key);

            frameProcessor.process(frame, keyHandler.getMode());

            if (keyHandler.getMode() == Mode::DRAW) {
                if (mData.drawing || mData.readyToDraw) {
                    cv::rectangle(frame, mData.start, mData.end, cv::Scalar(0, 0, 255), 2);
                }
                
                cv::line(frame, cv::Point(mData.end.x - 15, mData.end.y), 
                         cv::Point(mData.end.x + 15, mData.end.y), cv::Scalar(0, 255, 0), 1);
                cv::line(frame, cv::Point(mData.end.x, mData.end.y - 15), 
                         cv::Point(mData.end.x, mData.end.y + 15), cv::Scalar(0, 255, 0), 1);
            }

            display.drawOverlay(frame, keyHandler.getModeName(), cv::Point(15, 35));
            display.drawOverlay(frame, "FPS: " + std::to_string((int)fps), cv::Point(15, 65));
            
            std::string hint = "Controls: 0-5 (Modes) | Slider (Light) | Mouse (Draw) | ESC (Exit)";
            display.drawOverlay(frame, hint, cv::Point(15, frame.rows - 20));

            display.show(frame);
        }
    }

    return 0;
}
