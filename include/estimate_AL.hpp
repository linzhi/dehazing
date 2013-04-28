#ifndef ESTIMATE_AL_HPP
#define ESTIMATE_AL_HPP

/*
 * estimating the atmospheric light of each channel
 */

Scalar estimate_AL(Mat &img_b, Mat &img_g, Mat &img_r)
{
    double min_pixel = 0;
    double AL_B = 0, AL_G = 0, AL_R = 0;
    
    minMaxLoc(img_b, &min_pixel, &AL_B);
    minMaxLoc(img_g, &min_pixel, &AL_G);
    minMaxLoc(img_r, &min_pixel, &AL_R);

    return Scalar(AL_B, AL_G, AL_R);
}

#endif
