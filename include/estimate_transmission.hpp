#ifndef ESTIMATE_TRANSMISSION_HPP
#define ESTIMATE_TRANSMISSION_HPP

/*
 * estimating the transmission
 */

int estimate_transmission(const Mat &src, Mat &dst, Scalar A)
{
    // parameter for keep part of the haze to seem natural
    const double w = 0.95;
    double AL = 0;

    // choose the max value as atmospheric light
    AL = A.val[0] >= A.val[1] ? A.val[0] : A.val[1];
    AL = AL >= A.val[2] ? AL : A.val[2];

    uchar pixel;
    
    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            pixel = src.at<uchar>(i, j);
            dst.at<uchar>(i, j) = (1 - w * pixel / AL) * 255;
        }
    }
    
    return 1;
}

#endif
