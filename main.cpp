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
#include <dasiamtracker.h>
using namespace std;
using namespace cv;



//initial normalized global values for the static bbox, make them volatile to be able to change them with interrupts
volatile float bbox_x = 0.5;
volatile float bbox_y = 0.5;
volatile float bbox_w = 0.22;
volatile float bbox_h = 0.3;


//create a value for fps and score
volatile int fps, score;


//boolean value to update the static bbox using normalized global values for the static bbox, make it volatile to be able to change them with interrupts
volatile bool if_bbox_change = true;

//boolean value to check if tracking signal recieved, make it volatile to be able to change them with interrupts
volatile bool track = false;



int main( int argc, char** argv)
{


    //create matricies for frames
    Mat frame;


    //create the video capture object
    VideoCapture cap;


    //CHANGE THE CAPTURE OPTION HERE IF GSTREAMER DO NOT DESIRED TO BE USED
    //cap = VideoCapture(0);
    cap = VideoCapture("v4l2src device=/dev/video0 ! image/jpeg ! jpegdec ! videoconvert ! appsink");




    //if video couldn't captured, exit
    if (!cap.isOpened())
    {
        cout << "Error opening video file " << endl;
        return -1;
    }



    //get the capture dimensions
    int w = cap.get(CAP_PROP_FRAME_WIDTH);
    int h = cap.get(CAP_PROP_FRAME_HEIGHT);
    int org_fps = cap.get(CAP_PROP_FPS);


    DaSiamTracker* tracker = new DaSiamTracker(w,h);


    //create a value t detect the pressed key
    int f;



    //print original dimensions
    cout << "original w: " << w << ",  original h: " << h << ", original fps: " << org_fps << endl << endl;


    //read the first frame
    cap >> frame;

    //if fail to read the frame, exit
    if ( frame.empty() )
    { 
        cout << "Error reading the first frame" << endl;
        return -1; 
    }


    //set a named window
    namedWindow("Tracker", 1);


    //perform the tracking process
    printf("Start the tracking process, press ESC to quit, press t to track.\n\n");



    for( ;; )
    {

        
        //detect key
        f = waitKey(1);

        //if esc is pressed, break
        if(f == 27) break;

        //if t is pressed, start tracking
        if(f==116) track = true;



        //if there is a change in the bbox coordinates, update the static_bbox and downscaled_bbox
        if(if_bbox_change)
        {
            //update the static bbox coordinates
            tracker->set_static_bbox(bbox_x * w, bbox_y * h, bbox_w * w, bbox_h * h);

            //set if_bbox_change to false
            if_bbox_change = false;
        }  


        //get frame from the video
        cap >> frame;

        //if fail to read the frame
        if ( frame.empty() ) 
        { 
            cout << "Error while reading the frame" << endl << endl;
            return -1; 
        }


        //if track signal recieved, initialize tracking
        if(track)
        {

            tracker->set_frame(&frame);

            //initialize the tracker
            tracker->initialize_tracking();

            //print the message	
            cout << "Tracker initialized" << endl << endl;

            //set the track value to false again
            track = false;

        }



        // update the tracking result if tracking initialized
        if(tracker->if_tracking_initialized()) 
        {
            tracker->set_frame(&frame);
            
            //update the tracker
            tracker->track();

            //draw the tracked bbox
            rectangle( frame, tracker->get_current_tracked_bbox(), Scalar( 255, 0, 0 ), 2, 1 );
        }
        



        // draw the static bbox
        rectangle( frame, tracker->get_current_static_bbox(), Scalar( 0, 255, 0 ), 2, 1 );


        if(tracker->if_tracking_initialized()) //tracking already initialized
        {
            //put the fps to the frame
            putText(frame, //target image
                    "FPS:   " + to_string(tracker->get_fps()), //text
                    Point(30,30), //top-left position
                    FONT_HERSHEY_COMPLEX,
                    1.0,
                    Scalar(200, 0, 200), //font color
                    2);
            

            score = tracker->get_score() * 100;
            //put the fps to the frame
            if(tracker->get_score() < 60) //low score
            {
                putText(frame, //target image
                        "Tracking with:   %" + to_string(tracker->get_score()) + " score", //text
                        Point(30,80), //top-left position
                        FONT_HERSHEY_DUPLEX,
                        0.8,
                        Scalar(0, 0, 255), //font color
                        2);
            }
            else //high score
            {
                putText(frame, //target image
                        "Tracking with:   %" + to_string(tracker->get_score()) + " score", //text
                        Point(30,80), //top-left position
                        FONT_HERSHEY_DUPLEX,
                        0.8,
                        Scalar(0, 255, 0), //font color
                        2);
            }
        }
        else//tracking hasn't initialized
        {
            putText(frame, //target image
                    "Tracking has not initialized", //text
                    Point(30,80), //top-left position
                    FONT_HERSHEY_DUPLEX,
                    0.8,
                    Scalar(100, 100, 100), //font color
                    2);
        }
        

        // show image with the tracked object
        imshow("Tracker",frame);


    }
}
