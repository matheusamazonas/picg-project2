//
//  main.cpp
//  PICG-Project2
//
//  Created by Matheus Andrade on 8/20/14.
//  Copyright (c) 2014 Matheus Andrade. All rights reserved.
//
#include <iostream> // for standard I/O
#include <string>   // for strings
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

const char* inputFileName = "Wildlife.mp4";
const char* outputFilePath = "/Users/matheusandrade/Desktop/output.mp4";
VideoCapture inputVideo;
VideoWriter outputVideo;


void compare(Mat in, Mat out)
{
    imshow("Example-in", in);
    imshow("Example-out", out);
}

Mat blur(Mat image)
{
    Mat out = image.clone();
    GaussianBlur(image, out, Size(11,11), 0);
    return out;
}


int main(int argc, const char * argv[])
{
    Mat inFrame;
    Mat outFrame;
    cvNamedWindow("Derp Player");
    inputVideo = VideoCapture(inputFileName);
    Size frameSize = Size ((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),
                           (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
    int ex = CV_FOURCC('m', 'p', '4', 'v');

    outputVideo.open(
                     outputFilePath,
                     ex,
                     inputVideo.get(CV_CAP_PROP_FPS),
                     frameSize,
                     true);

    int c = 0;
    if (outputVideo.isOpened())
    {
        while (c < 100)
        {
            if (!inputVideo.read(inFrame)) break;
            outFrame = blur(inFrame);
            //outputVideo.write(inFrame);
            compare(inFrame, outFrame);
            printf("Frame # %i\n", c);
            c++;
        }
    }
    inputVideo.release();
    outputVideo.release();
    cvDestroyWindow("Derp Player");
    return 0;
}

