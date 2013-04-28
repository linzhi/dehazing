#ifndef ESTIMATE_TRANSMISSION_HPP
#define ESTIMATE_TRANSMISSION_HPP

/*
 * estimating the transmission
 */

int estimate_transmission(const IplImage *dark_channel, IplImage *transmission)
{
    // parameter for keep part of the haze to seem natural
    const double w = 0.95;
    double AL = 0;

    // choose the max value as atmospheric light
    AL = AL_B >= AL_G ? AL_R : AL_G;
    AL = AL >= AL_R ? AL : AL_R;

    CvScalar pixel_in, pixel_out;
    
    for (int i = 0; i < dark_channel->height; i++) {
        for (int j = 0; j < dark_channel->width; j++) {
            pixel_in = cvGet2D(dark_channel, i, j);
            pixel_out = cvScalar((1 - w * pixel_in.val[0] / AL) * 255, 0, 0, 0);
            cvSet2D(transmission, i, j, pixel_out);
        }
    }

    return 1;
}

#endif
