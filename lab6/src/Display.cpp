#include "Display.hpp"

Display::Display(const std::string& name) : windowName(name) {
    cv::namedWindow(windowName, cv::WINDOW_AUTOSIZE);
}

void Display::show(const cv::Mat& frame) {
    if (!frame.empty()) cv::imshow(windowName, frame);
}

void Display::drawOverlay(cv::Mat& frame, const std::string& text, cv::Point pos) {
    cv::putText(frame, text, pos, cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
}

void Display::setupMouse(MouseData* data) {
    cv::setMouseCallback(windowName, mouseCallback, data);
}

void Display::setupTrackbar(int* value, int maxVal) {
    cv::createTrackbar("Brightness", windowName, value, maxVal);
}

void Display::mouseCallback(int event, int x, int y, int flags, void* userdata) {
    MouseData* data = (MouseData*)userdata;
    if (event == cv::EVENT_LBUTTONDOWN) {
        data->drawing = true;
        data->start = cv::Point(x, y);
        data->readyToDraw = false;
    } else if (event == cv::EVENT_MOUSEMOVE) {
        if (data->drawing) data->end = cv::Point(x, y);
    } else if (event == cv::EVENT_LBUTTONUP) {
        data->drawing = false;
        data->end = cv::Point(x, y);
        data->readyToDraw = true;
    }
}
