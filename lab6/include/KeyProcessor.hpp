#ifndef KEY_PROCESSOR_HPP
#define KEY_PROCESSOR_HPP

#include <opencv2/opencv.hpp>

enum class Mode {
    NORMAL,
    INVERT,
    CANNY,
    GLITCH,
    SOBEL,
    DRAW
};

class KeyProcessor {
private:
    Mode currentMode;

public:
    KeyProcessor();
    
    void handleKey(int key);
    
    Mode getMode() const;
    std::string getModeName() const;
};

#endif
