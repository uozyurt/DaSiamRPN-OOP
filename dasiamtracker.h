#ifndef TRACKER_H
#define TRACKER_H

#include <opencv2/opencv.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <sys/time.h>

using namespace cv;

class DaSiamTracker
{
public:
    //constructors
    DaSiamTracker();
    DaSiamTracker(int w, int h);



    //function to initialize the tracking with the current static bbox
    void initialize_tracking();

    //function to continue tracking
    void track();


    //function to get the new frame to be used in tracking
    void set_frame(Mat* new_frame);



    //functions for setting the new static bbox, which will be used in tracking initialization at each track signal
    void set_static_bbox(int new_bbox_x_px, int new_bbox_y_px, int new_bbox_w_px, int new_bbox_h_px);

    //function to set the width and height of the frames
    void set_w_h(int new_w, int new_h);



    //function to get the current tracked bbox to display in the screen
    Rect get_current_tracked_bbox();
    
    Rect get_current_static_bbox();


    //function to get the current fps
    int get_fps();

    //function to get the current score
    int get_score();

    //function to check if tracking initialized
    bool if_tracking_initialized();


private:

    //width and height of the current capture
    int w;
    int h;


    //normalized values for the static bbox, make them volatile to be able to change them with interrupts
    volatile float bbox_x;
    volatile float bbox_y;
    volatile float bbox_w;
    volatile float bbox_h;

    //actual pixel values for the static bbox
    int bbox_x_px;
    int bbox_y_px;
    int bbox_w_px;
    int bbox_h_px;


    //boolean value to check if tracking ever initialized.
    bool if_initialized;

    //create a value for fps and score
    volatile int fps, score;

    //create a matrice pointer for frame
    Mat* frame;

    //create rectangle objects for static_bbox, tracked_bbox and downscaled_bbox
    Rect static_bbox, tracked_bbox;

    //create the tracker object
    Ptr<TrackerDaSiamRPN> tracker;

    //create time objects
    struct timeval start_time, stop_time;
};

#endif // TRACKER_H

