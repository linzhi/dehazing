#ifndef ESTIMATE_AL_HPP
#define ESTIMATE_AL_HPP

/*
 * estimating the atmospheric light of each channel
 */

double AL_B = 0, AL_G = 0, AL_R = 0;

CvScalar estimate_AL(IplImage *img_b, IplImage *img_g, IplImage *img_r)
{
    double min_pixel = 0;
    cvMinMaxLoc(img_b, &min_pixel, &AL_B, NULL, NULL, NULL);
    cvMinMaxLoc(img_g, &min_pixel, &AL_G, NULL, NULL, NULL);
    cvMinMaxLoc(img_r, &min_pixel, &AL_R, NULL, NULL, NULL);

    return cvScalar(AL_B, AL_G, AL_R, 0);
}

#endif
