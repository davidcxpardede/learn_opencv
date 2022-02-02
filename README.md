# learn_opencv

## Open-CV installation (Ubuntu 20.04 LTS)
This workspace c++ openCV version: 4.5.5
1. Install dependencies

```
    sudo apt install build-essential cmake git pkg-config libgtk-3-dev \
    libavcodec-dev libavformat-dev libswscale-dev libv4l-dev \
    libxvidcore-dev libx264-dev libjpeg-dev libpng-dev libtiff-dev \
    gfortran openexr libatlas-base-dev python3-dev python3-numpy \
    libtbb2 libtbb-dev libdc1394-22-dev libopenexr-dev \
    libgstreamer-plugins-base1.0-dev libgstreamer1.0-dev

```

2. Clone the OpenCV’s and OpenCV contrib repositories
```mkdir ~/opencv_build && cd ~/opencv_build```
```git clone https://github.com/opencv/opencv.git```
```git clone https://github.com/opencv/opencv_contrib.git```

3. Create a temporary build directory
```cd ~/opencv_build/opencv```
```mkdir -p build && cd build```

4. Set up the OpenCV build with CMake
```
    cmake -D CMAKE_BUILD_TYPE=RELEASE \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_C_EXAMPLES=ON \
    -D INSTALL_PYTHON_EXAMPLES=ON \
    -D OPENCV_GENERATE_PKGCONFIG=ON \
    -D OPENCV_EXTRA_MODULES_PATH=~/opencv_build/opencv_contrib/modules \
    -D BUILD_EXAMPLES=ON ..
```

5. Start the compilation process:

```make -jN```
where N is the max number thread in your PC, or you may remove the flag.

```sudo make install```

6. Verify the installation
C++: ```pkg-config --modversion opencv4```
Python: ```python3 -c "import cv2; print(cv2.__version__)"```

7. Verify opencv build directories installation
```sudo ls /usr/local/include/opencv4``` with expected outcome ```opencv2```

## Run the program
1. Navigate to an example directory ```cd src/modify_image```

2. Compile with ```make```

3. Run with ```make run```

4. Clear compilled object ```make clean```