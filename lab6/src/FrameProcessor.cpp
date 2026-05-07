#include "FrameProcessor.hpp"

void FrameProcessor::process(cv::Mat& frame, Mode mode) {
    switch (mode) {
        case Mode::INVERT: applyInvert(frame); break;
        case Mode::CANNY:  applyCanny(frame);  break;
        case Mode::GLITCH: applyGlitch(frame); break;
        case Mode::SOBEL:  applySobel(frame);  break;
        default: break;
    }
}

void FrameProcessor::applyInvert(cv::Mat& frame) {
    cv::bitwise_not(frame, frame);
}

void FrameProcessor::applyCanny(cv::Mat& frame) {
    cv::Mat gray, edges;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 100, 200);
    cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);
}

void FrameProcessor::applySobel(cv::Mat& frame) {
    cv::Mat gray, grad_x, grad_y, abs_grad_x, abs_grad_y;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Sobel(gray, grad_x, CV_16S, 1, 0);
    cv::Sobel(gray, grad_y, CV_16S, 0, 1);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, frame);
    cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR);
}

void FrameProcessor::applyGlitch(cv::Mat& frame) {
    std::vector<cv::Mat> channels;
    cv::split(frame, channels);

    int offset = 10;
    
    cv::Mat blueShifted = cv::Mat::zeros(channels[0].size(), channels[0].type());
    cv::Mat redShifted = cv::Mat::zeros(channels[2].size(), channels[2].type());

    channels[0](cv::Rect(offset, 0, channels[0].cols - offset, channels[0].rows))
        .copyTo(blueShifted(cv::Rect(0, 0, channels[0].cols - offset, channels[0].rows)));
        
    channels[2](cv::Rect(0, 0, channels[2].cols - offset, channels[2].rows))
        .copyTo(redShifted(cv::Rect(offset, 0, channels[2].cols - offset, channels[2].rows)));

    std::vector<cv::Mat> glitchChannels = {blueShifted, channels[1], redShifted};
    cv::merge(glitchChannels, frame);
}
