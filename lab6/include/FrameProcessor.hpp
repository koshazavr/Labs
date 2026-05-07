#ifndef FRAME_PROCESSOR_HPP
#define FRAME_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor {
public:
    void process(cv::Mat& frame, Mode mode);

private:
    void applyGlitch(cv::Mat& frame);
    void applyCanny(cv::Mat& frame);
    void applySobel(cv::Mat& frame);
    void applyInvert(cv::Mat& frame);
};

#endif
