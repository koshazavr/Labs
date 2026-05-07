#include "KeyProcessor.hpp"

KeyProcessor::KeyProcessor() : currentMode(Mode::NORMAL) {}

void KeyProcessor::handleKey(int key) {
    if (key == -1) return; // Клавіша не натиснута

    switch (key) {
        case '0': currentMode = Mode::NORMAL; break;
        case '1': currentMode = Mode::INVERT; break;
        case '2': currentMode = Mode::CANNY;  break;
        case '3': currentMode = Mode::GLITCH; break;
        case '4': currentMode = Mode::SOBEL;  break;
        case '5': currentMode = Mode::DRAW;   break;
    }
}

Mode KeyProcessor::getMode() const { return currentMode; }

std::string KeyProcessor::getModeName() const {
    switch (currentMode) {
        case Mode::INVERT: return "Mode: Inversion";
        case Mode::CANNY:  return "Mode: Canny Edges";
        case Mode::GLITCH: return "Mode: Glitch Effect";
        case Mode::SOBEL:  return "Mode: Sobel Filter";
        case Mode::DRAW:   return "Mode: Drawing";
        default:           return "Mode: Normal";
    }
}
