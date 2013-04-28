#include "opencv2/opencv.hpp"
#include <iostream>
#include <algorithm>

using namespace std;
using namespace cv;

#include "include/dark_channel.hpp"
#include "include/estimate_AL.hpp"
#include "include/estimate_transmission.hpp"
#include "include/dehazing.hpp"

int main(int argc, char *argv[])
{
    IplImage *haze_img = NULL;
    IplImage *haze_img_b = NULL;
    IplImage *haze_img_g = NULL;
    IplImage *haze_img_r= NULL;
    IplImage *dark_channel = NULL;
    IplImage *transmission = NULL;
    IplImage *dehaze_img = NULL;

    if (argc == 1)
        cout << "Usage: ./dehaze filename" << endl;
    else if (argc == 2 && (haze_img = cvLoadImage(argv[1], 1)) != 0) {
        cvNamedWindow("haze image", 1);
        cvShowImage("haze image", haze_img);
    }

    haze_img_b = cvCreateImage(cvGetSize(haze_img), IPL_DEPTH_8U, 1);
    haze_img_g = cvCreateImage(cvGetSize(haze_img), IPL_DEPTH_8U, 1);
    haze_img_r = cvCreateImage(cvGetSize(haze_img), IPL_DEPTH_8U, 1);
    dark_channel = cvCreateImage(cvGetSize(haze_img), IPL_DEPTH_8U, 1);
    transmission = cvCreateImage(cvGetSize(haze_img), IPL_DEPTH_8U, 1);
    dehaze_img = cvCreateImage(cvGetSize(haze_img), IPL_DEPTH_8U, 3);

    cvSplit(haze_img, haze_img_b, haze_img_g, haze_img_r, NULL);

    // atmospheric light A
    CvScalar A = estimate_AL(haze_img_b, haze_img_g, haze_img_r);

    // get dark channel of hazy image
    get_dark_channel(haze_img, dark_channel);

    // get transmission diagram
    estimate_transmission(dark_channel, transmission);

    // get haze free image
    dehazing(haze_img, transmission, dehaze_img, A);

    cvNamedWindow("haze image", 1);
    cvShowImage("haze image", haze_img);

    cvNamedWindow("dark channel", 1);
    cvShowImage("dark channel", dark_channel);

    cvNamedWindow("transmission image", 1);
    cvShowImage("transmission image", transmission);

    cvNamedWindow("haze free image", 1);
    cvShowImage("haze free image", dehaze_img);
    
    cvWaitKey(0);

    cvDestroyWindow("haze image");
    cvDestroyWindow("dark channel");
    cvDestroyWindow("transmission image");
    cvDestroyWindow("haze free image");

    cvReleaseImage(&haze_img);
    cvReleaseImage(&haze_img_b);
    cvReleaseImage(&haze_img_g);
    cvReleaseImage(&haze_img_r);
    cvReleaseImage(&dark_channel);
    cvReleaseImage(&transmission);
    cvReleaseImage(&dehaze_img);

    return 0;
}










