#!/bin/bash

mkdir -p build
cd build

cmake ..
make

echo "Збірка завершена. Програма знаходиться в build/opencv_lab"
