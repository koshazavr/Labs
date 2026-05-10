cmake_minimum_required(VERSION 3.10)
project(opencv_lab)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories(include)

set(SOURCES 
    src/main.cpp 
    src/CameraProvider.cpp 
    src/Display.cpp 
    src/KeyProcessor.cpp 
    src/FrameProcessor.cpp
    src/FaceDetector.cpp
)

add_executable(opencv_lab ${SOURCES})
target_link_libraries(opencv_lab ${OpenCV_LIBS})
