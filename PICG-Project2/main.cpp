//
//  main.cpp
//  PICG-Project2
//
//  Created by Matheus Andrade on 8/20/14.
//  Copyright (c) 2014 Matheus Andrade. All rights reserved.
//
//  Filters: blur, laplace, median
//  Aritmetic operands:
//
//  Operadores pontuais: negativo, threshold, grayscale
//  Operadores aritméticos: 
//  Filtros: blur (borramento), sharpening (alto-reforço) median (mediana)

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

const char* inputFileName = "Wildlife.mp4";
const char* outputFilePath = "/Users/matheusandrade/Desktop/output.mp4";
VideoCapture inputVideo;
VideoWriter outputVideo;


Mat blur(Mat image, int size)
{
    Mat out = image.clone();
    Mat kernel = Mat::ones(size, size, CV_32F )/ (float)(size*size);
    filter2D(image, out, -1, kernel);
    return out;
}

Mat negative(Mat image)
{
    Mat out;
    subtract(255, image , out);
    return out;
}

Mat sharpening(Mat image, int size, float originalFactor = 1.0f, int neighbour = 1, float centerOffset = 0)
{
    Mat out = image.clone();
    Mat kernel = Mat(size, size, CV_32F, Scalar(-neighbour));
    int center = int(size/2);
    kernel.at<float>(center, center) = (size * size * neighbour) - centerOffset;
    filter2D(image, out, -1, kernel);
    
    out = (originalFactor * image) + ((1 - originalFactor) * out);
    
    return out;
}

Mat median(Mat image, int size)
{
    Mat out = image.clone();
    
    medianBlur(image, out, size);
    return out;
}

Mat grayScale(Mat image)
{
    Mat out = image.clone();
    
    cvtColor(image, out, CV_RGB2GRAY);
    return out;
}


void init ()
{
    inputVideo = VideoCapture(inputFileName);
    int videoWidth = (int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH);
    int videoHeight =( int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
    Size frameSize = Size (videoWidth, videoHeight);
    
    namedWindow("Example-in");
    namedWindow("Program 1");
    namedWindow("Program 2");
    moveWindow("Example-in", 0, 0);
    moveWindow("Program 1", videoWidth, 0);
    moveWindow("Program 2", 0, videoHeight + 45);
    
    int ex = CV_FOURCC('m', 'p', '4', 'v');
    
    outputVideo.open(
                     outputFilePath,
                     ex,
                     inputVideo.get(CV_CAP_PROP_FPS),
                     frameSize,
                     true);
}

Mat program1 (Mat image)
{
    int threshold_value = 100;
    int threshold_type = 0;
    int const max_BINARY_value = 255;
    
    Mat out = sharpening(image, 50, 0.0f, 1, -50);
    out = blur(out, 3);
    out = grayScale(out);
    threshold( out, out, threshold_value, max_BINARY_value, threshold_type );
    return out;
}

Mat program2 (Mat image)
{
    Mat out = median(image, 13);
    out = sharpening(out, 5, 0.6f, 1);
    out *= 1.2f;
    return out;
}

int main(int argc, const char * argv[])
{
    Mat inFrame;
    Mat outFrame;
    
    init();
    
    //int c = 0;
    if (outputVideo.isOpened())
    {
        while (1)
        {
            if (!inputVideo.read(inFrame)) break;
            outFrame = inFrame.clone();
            imshow("Example-in", inFrame);
            imshow("Program 1", program1(inFrame));
            imshow("Program 2", program2(inFrame));
            //outputVideo.write(outFrame);
            char c = cvWaitKey(1);
            if( c == 27 ) break;
        }
    }
    
    inputVideo.release();
    outputVideo.release();
    return 0;
}

