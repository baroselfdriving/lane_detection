/**
 * Detect.cpp
 * Provides functions for lane detection
 * 
 * @author Oscar Morales Ponce
 * @author Jared Coleman
 * @version 1.0 12/02/17
 */

using namespace std;

#include <string>
#include <libconfig.h++>

#include "opencv2/opencv.hpp"

#include "lane.h"
#include "detector.h"

using namespace cv;
   
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <config file>" << endl;
        return 0;
    }
    
    string config_path(argv[1]);
    
    string video_path;
    try
    {
        libconfig::Config cfg;
        cfg.readFile(config_path.c_str());

        video_path = cfg.lookup("video.file").c_str();
    }
    catch(...)
    {
        cerr << "Invalid config file" << endl;
        return 0;
    }
    
    VideoCapture cap(video_path);
    Lane lane(config_path);
    Detector detector(config_path);
    
    if(!cap.isOpened()) return -1;
    
    namedWindow("output", 1);
    Mat frame;
    cap >> frame;
    
    while(true)
    {
        try
        {
            //get frame from stream
            cap >> frame;
            detector.getLanes(frame, lane);
            
            //draw lanes
            detector.drawLane(frame, lane);
            
            //show image
            imshow("output", frame);
            if(waitKey(1) >= 0) break;
        }
        catch(cv::Exception e)
        {
            cout << e.what() << endl; 
            break;
        }
    }
}
