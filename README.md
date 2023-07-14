# DaSiamRPN-OOP

This is an openCV DaSiamRPN implementation. It is tested with NVIDIA Jetson Nano and recieved an average of 16-19 FPS. With NVIDIA RTX 3050 mobile version, it recieves around 120-130 FPS.\
Note: The current implementation is using CUDA backend with fp16 halp precision in order to increase FPS.\
\
**!!WARNING:** To build and run this code, you have to build openCV with CUDA. [This link](https://github.com/zadobudak/Install-Opencv-4.7.0-CUDA-Ubuntu) can be used for that purpose.\
\
**WARNING:** The .onnx files in the build directory is essential for the code to run. Do not delete them.\
\
**WARNING:**  It is tested on opencv 4.7 and 4.6 but the current code is for the 4.7 version. If the code needs to be used with opencv 4.6, then `#include <opencv2/video/tracking.hpp>` line should be changed with  `#include <opencv2/tracking.hpp>`\
\
This implementation is made for easy usage of the DaSiamRPN tracker with the utilization of abstraction.\
An example implementation of the **DaSiamTracker** class in dasiamtracker.hpp file can be tested with building the main.cpp file as following:


### Building and Running
First, download the files
Then, go in to the project folder. Navigate to the build directory.
```sh
cd build
```
After this, perform cmake and make commands.
```sh
cmake ..
make
```
Finally, you are ready to run the program.
```sh
./DaSiamRPNTrack
```
