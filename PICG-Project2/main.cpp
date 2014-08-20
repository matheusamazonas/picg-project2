//
//  main.cpp
//  PICG-Project2
//
//  Created by Matheus Andrade on 8/20/14.
//  Copyright (c) 2014 Matheus Andrade. All rights reserved.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

const char* sampleFileName = "sample1.avi";

int main(int argc, const char * argv[])
{
    CvCapture *capture = cvCreateFileCapture(sampleFileName);
    if (capture == NULL)
    {
        printf("Error while loading file %s\n", sampleFileName);
    }
    
    int c = 0;
    while (cvGrabFrame(capture) != 0)
    {
        c++;
    }
    int frameCount = cvGetCaptureProperty(capture, 7);
    printf("Frames reading the entire file: %i \nFrames accessing frameCount: %i\n", c, frameCount);
    cvReleaseCapture(&capture);
    
    return 0;
}

