#include "dasiamtracker.h"
#include <iostream>
using namespace std;
using namespace cv;



//initialize the tracker
DaSiamTracker::DaSiamTracker()
{
    //set the if_initialized valut to false
    this->if_initialized = false;


    //create the tracker
    TrackerDaSiamRPN::Params params;
    params.model = samples::findFile("dasiamrpn_model.onnx");
    params.kernel_cls1 = samples::findFile("dasiamrpn_kernel_cls1.onnx");
    params.kernel_r1 = samples::findFile("dasiamrpn_kernel_r1.onnx");

    /*
        "{ backend     | 0 | Choose one of computation backends: "
                            "0: automatically (by default), "
                            "1: Halide language (http://halide-lang.org/), "
                            "2: Intel's Deep Learning Inference Engine (https://software.intel.com/openvino-toolkit), "
                            "3: OpenCV implementation, "
                            "4: VKCOM, "
                            "5: CUDA },"
        "{ target      | 0 | Choose one of target computation devices: "
                            "0: CPU target (by default), "
                            "1: OpenCL, "
                            "2: OpenCL fp16 (half-float precision), "
                            "3: VPU, "
                            "4: Vulkan, "
                            "6: CUDA, "
                            "7: CUDA fp16 (half-float preprocess) }"
    */
    params.backend = 5; //CUDA Backend

    //CHANGE THE TARGET VALUE TO 7 IF FULL FLOAT PRECISION IS DESIRED
    params.target = 7; //CUDA fp16 (half-float preprocess)

    tracker = TrackerDaSiamRPN::create(params);


    //initialize the tracker on an arbitrary useless frame
    tracker->init(Mat(320, 240, CV_8UC3, cv::Scalar(0, 0, 0)), Rect(10,10,10,10));

}

//initialize the tracker
DaSiamTracker::DaSiamTracker(int w, int h)
{
    //set the width and height values
    this->set_w_h(w,h);

    //set default static bbox coordinated
    float initial_static_x = 0.5;
    float initial_static_y = 0.5;
    float initial_static_w = 0.16;
    float initial_static_h = 0.25;


    //set the static box
    this->set_static_bbox(initial_static_x * w, initial_static_y * h, initial_static_w * w, initial_static_h * h);

    //set the if_initialized valut to false
    this->if_initialized = false;


    //create the tracker
    TrackerDaSiamRPN::Params params;
    params.model = samples::findFile("dasiamrpn_model.onnx");
    params.kernel_cls1 = samples::findFile("dasiamrpn_kernel_cls1.onnx");
    params.kernel_r1 = samples::findFile("dasiamrpn_kernel_r1.onnx");

    /*
        "{ backend     | 0 | Choose one of computation backends: "
                            "0: automatically (by default), "
                            "1: Halide language (http://halide-lang.org/), "
                            "2: Intel's Deep Learning Inference Engine (https://software.intel.com/openvino-toolkit), "
                            "3: OpenCV implementation, "
                            "4: VKCOM, "
                            "5: CUDA },"
        "{ target      | 0 | Choose one of target computation devices: "
                            "0: CPU target (by default), "
                            "1: OpenCL, "
                            "2: OpenCL fp16 (half-float precision), "
                            "3: VPU, "
                            "4: Vulkan, "
                            "6: CUDA, "
                            "7: CUDA fp16 (half-float preprocess) }"
    */
    params.backend = 5; //CUDA Backend

    //CHANGE THE TARGET VALUE TO 7 IF FULL FLOAT PRECISION IS DESIRED
    params.target = 7; //CUDA fp16 (half-float preprocess)

    tracker = TrackerDaSiamRPN::create(params);


    //initialize the tracker on an arbitrary useless frame
    tracker->init(Mat(320, 240, CV_8UC3, cv::Scalar(0, 0, 0)), Rect(10,10,10,10));

}


//initialize the tracking with the current frame and current static_bbox
void DaSiamTracker::initialize_tracking()
{
    //initialize the tracker
    tracker->init(*frame, static_bbox);


    //set if_initialized to true
    if_initialized = true;
}



//continue to tracking
void DaSiamTracker::track()
{
    //start the timer
    gettimeofday(&start_time, NULL);

    //update the tracker
    tracker->update(*frame,tracked_bbox);

    //end the timer
    gettimeofday(&stop_time, NULL);

    //calculate the fps
    fps = 1000000 / ((stop_time.tv_sec * 1000000 + stop_time.tv_usec) - (start_time.tv_sec * 1000000 + start_time.tv_usec));

    //calculate thre score
    score = tracker->getTrackingScore() * 100;
}





//update the static bbox with new actual pixel values
void DaSiamTracker::set_static_bbox(int new_bbox_x_px, int new_bbox_y_px, int new_bbox_w_px, int new_bbox_h_px)
{
    bbox_x = (float)new_bbox_x_px / this->w;
    bbox_y = (float)new_bbox_y_px / this->h;
    bbox_w = (float)new_bbox_w_px / this->w;
    bbox_h = (float)new_bbox_h_px / this->h;

    bbox_x_px = new_bbox_x_px;
    bbox_y_px = new_bbox_y_px;
    bbox_w_px = new_bbox_w_px;
    bbox_h_px = new_bbox_h_px;


    //update the static_bbox
    static_bbox = Rect(bbox_x_px - bbox_w_px/2 ,bbox_y_px - bbox_h_px/2, bbox_w_px, bbox_h_px);
}



//update the width-height of the frame
void DaSiamTracker::set_w_h(int new_w, int new_h)
{
    w = new_w;
    h = new_h;
}



//set the new frame
void DaSiamTracker::set_frame(Mat* new_frame)
{
    frame = new_frame;
}



//get the current tracked bbox
Rect DaSiamTracker::get_current_tracked_bbox()
{
    return tracked_bbox;
}

//get the current static bbox
Rect DaSiamTracker::get_current_static_bbox()
{
    return static_bbox;
}


//get the current fps
int DaSiamTracker::get_fps()
{
    return this->fps;
}



//get the current score
int DaSiamTracker::get_score()
{
    return this->score;
}



//get the booleand value of if tracking initialized
bool DaSiamTracker::if_tracking_initialized()
{
    return if_initialized;
}


