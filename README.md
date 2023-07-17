# DaSiamRPN-OOP

This is an openCV DaSiamRPN object oriented implementation. An example usage is made in the main.cpp file. Besides that, the DaSiamTracker class methods are explained in the **Class Usage Explanation**. It is tested with NVIDIA Jetson Nano and recieved an average of 16-19 FPS. With NVIDIA RTX 3050 mobile version, it recieves around 120-130 FPS.\
Note: The current implementation is using CUDA backend with fp16 halp precision in order to increase FPS. The backend and target can be changed in dasiamtracker.cpp file.\
\
**!!WARNING:** To build and run this code, you have to build openCV with CUDA. [This link](https://github.com/zadobudak/Install-Opencv-4.7.0-CUDA-Ubuntu) can be used for that purpose.\
\
**WARNING:** The .onnx files in the build directory is essential for the code to run. Do not delete them.\
\
**WARNING:**  It is tested on opencv 4.7 and 4.6 but the current code is for the 4.7 version. If the code needs to be used with opencv 4.6, then `#include <opencv2/video/tracking.hpp>` line should be changed with  `#include <opencv2/tracking.hpp>`\
\
This implementation is made for easy usage of the DaSiamRPN tracker with the utilization of abstraction.\
An example implementation of the **DaSiamTracker** class in dasiamtracker.hpp file can be tested with building the main.cpp file as following:

### Class Usage Explanation
This DaSiamTracker class implementation is made for the sake of simplicity, using abstraction as much as possible. To initialize the tracking, you need to follow the steps below :

#### Summary of the usage explanation:
1. Create the tracker object, set it's width and height. Also declare the initial bounding box coordinates.
2. To initialize, set the current frame and call the initialize tracking function.
3. To resume the tracking, set the current frame and call the track function.
4. Use `get_current_tracked_bbox()`, `get_fps()`, `get_score()` functions to obtain the results.
**!! WARNING: Before initialization and each single tracking operation, you should set a new frame.**

#### More detailed usage explanation:
(It is assumed that the video capture is already handled and you can get consecutive frames in your program.)
1. Obtain the capture width and height of the frames.
2. Create the tracker object using the capture width and height, either with "**DaSiamTracker(int w, int h)**" contructor or with the "**DaSiamTracker()**" contructor and use "**set_w_h(int new_w, int new_h)**" function.
3. Use "**void set_static_bbox(int new_bbox_x_px, int new_bbox_y_px, int new_bbox_w_px, int new_bbox_h_px)**" funtion to declare the static bounding box, where the tracking will be initialized. (Use pixel values as parameters. First two parameters are for the center of the bounding box, the last two are for the width and height of the bounding box.)
4. Before initializing the tracker, use "**set_frame(Mat* new_frame)**" function to load the current frame in to the tracker object, and call "**initialize_tracking()**" function. **(Pass the address of the Mat object)**
5. For each track operation after initialization, it is necessary to use "**set_frame(Mat* new_frame)**" function. After this, you can call the "**track()**" function.
6. After each track function is called, you can obtain the resulting bounding box with "**get_current_tracked_bbox()**" function, and get the tracking score and fps with "**get_score()**" and "**get_fps()**" functions respectively.


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
