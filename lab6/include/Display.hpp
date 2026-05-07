#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <opencv2/opencv.hpp>
#include <string>

struct MouseData {
    cv::Point start, end;
    bool drawing = false;
    bool readyToDraw = false;
};

class Display {
private:
    std::string windowName;
    static void mouseCallback(int event, int x, int y, int flags, void* userdata);

public:
    Display(const std::string& name = "Lab 6 - OpenCV");
    void show(const cv::Mat& frame);
    void drawOverlay(cv::Mat& frame, const std::string& text, cv::Point pos);
    
    void setupMouse(MouseData* data);
    void setupTrackbar(int* value, int maxVal);
};

#endif
