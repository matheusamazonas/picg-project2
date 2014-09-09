//  Created by Matheus Andrade on 8/20/14.
//  Copyright (c) 2014 Matheus Andrade. All rights reserved.
//
//  Operadores pontuais: negativo, threshold, grayscale
//  Operadores aritméticos:
//  Filtros: blur (borramento), sharpening (alto-reforço) median (mediana)

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

// Input file name (path is the current one
const char* inputFileName = "Wildlife.mp4";

// Output file paths
const char* outputFilePath1 = "/Users/matheusandrade/Desktop/output1.mp4";
const char* outputFilePath2 = "/Users/matheusandrade/Desktop/output2.mp4";

VideoCapture inputVideo;
VideoWriter outputVideo1;
VideoWriter outputVideo2;

// Export mode true saves the files on the given paths.
// Export mode false shows the videos on the screen
bool exportMode = false;

// Blur effect using avarage filter
Mat blur(Mat image, int size)
{
    Mat out = image.clone();
    Mat kernel = Mat::ones(size, size, CV_32F )/ (float)(size*size);
    filter2D(image, out, -1, kernel);
    return out;
}

// negative filter to a 8-bit image
Mat negative(Mat image)
{
    Mat out;
    subtract(255, image , out);
    return out;
}

// Sharpening filter
//  size = filter size (size*size)
//  originalFactor = how much of the original image will be blended
//  neighbour = value to be used to every filter element but the center
//  center offset (center - offset) (A)
Mat sharpening(Mat image, int size, float originalFactor = 1.0f, int neighbour = 1, float centerOffset = 0)
{
    Mat out = image.clone();
    Mat kernel = Mat(size, size, CV_32F, Scalar(-neighbour));
    int center = int(size/2);
    kernel.at<float>(center, center) = (size * size * neighbour) - centerOffset;
    filter2D(image, out, -1, kernel);
    
    // Blending
    out = (originalFactor * image) + ((1 - originalFactor) * out);
    
    return out;
}

// Median filter
//  size = filter size (size*size)
Mat median(Mat image, int size)
{
    Mat out = image.clone();
    
    medianBlur(image, out, size);
    return out;
}

// GrayScale operation
Mat grayScale(Mat image)
{
    Mat out = image.clone();
    cvtColor(image, out, CV_RGB2GRAY);
    // I need to convert back to RGB because the VideoWriter doesn't support
    //  grayscale images on MacOS (available only on Windows)
    cvtColor(out, out, CV_GRAY2RGB);
    return out;
}

// Initialization. Load input file and frame size
// If it's export mode, open the output files and checks if it's okay
// If it's visual mode, creates and positions the windows
void init ()
{
    inputVideo = VideoCapture(inputFileName);
    int videoWidth = (int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH);
    int videoHeight =( int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
    Size frameSize = Size (videoWidth, videoHeight);

    if (exportMode)
    {
        int ex = CV_FOURCC('m', 'p', '4', 'v');

        // Even though the video is a grayscale video, the last parameter is
        //  true because OpenCV doesn't support grayscale video writing on
        //  MacOS (Windows only)
        outputVideo1.open(
                          outputFilePath1,
                          ex,
                          inputVideo.get(CV_CAP_PROP_FPS),
                          frameSize,
                          true);
        outputVideo2.open(
                          outputFilePath2,
                          ex,
                          inputVideo.get(CV_CAP_PROP_FPS),
                          frameSize,
                          true);
        if (!outputVideo1.isOpened() || !outputVideo2.isOpened())
        {
            printf("Error while opening the output files\n");
            exit(-1);
        }
    }
    else
    {
        namedWindow("Original input");
        namedWindow("Program 1");
        namedWindow("Program 2");
        moveWindow("Original input", 0, 0);
        moveWindow("Program 1", videoWidth, 0);
        moveWindow("Program 2", 0, videoHeight + 45);
    }

}

// Program1 simulates a WoodCut effect (xilogravura) using a sharpening filter,
//  a blur filter, a grayscale operation, a threshold and a negative operation
Mat program1 (Mat image)
{
    int threshold_value = 100;
    int threshold_type = 1;
    int const max_BINARY_value = 255;
    
    Mat out = sharpening(image, 50, 0.0f, 1, -50);
    out = blur(out, 3);
    Mat gray = grayScale(out);
    threshold(gray, gray, threshold_value, max_BINARY_value, threshold_type);
    out = negative(gray);
    return out;
}

// Program2 simulates a painting effect (Monet style) using a median filter,/
//  a sharpening filter and a brightness increase
Mat program2 (Mat image)
{
    Mat out = median(image, 13);
    out = sharpening(out, 5, 0.6f, 1);
    out *= 1.2f;
    return out;
}

// Main
int main(int argc, const char * argv[])
{
    Mat inFrame;
    Mat outFrame1;
    Mat outFrame2;
    
    // Initialization
    init();
    
    // While there are frames available, either export or show the video
    while (1)
    {
        if (!inputVideo.read(inFrame)) break;
        outFrame1 = program1(inFrame);
        outFrame2 = program2(inFrame);
        if (exportMode)
        {
            outputVideo1.write(outFrame1);
            outputVideo2.write(outFrame2);
        }
        else
        {
            imshow("Original input", inFrame);
            imshow("Program 1", outFrame1);
            imshow("Program 2", outFrame2);
            
            // To exit visualization mode, press ESC
            char c = cvWaitKey(1);
            if( c == 27 ) break;
        }
    }
    
    inputVideo.release();
    outputVideo1.release();
    outputVideo2.release();
    return 0;
}

