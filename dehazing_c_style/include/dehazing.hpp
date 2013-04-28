#ifndef DEHAZING_HPP
#define DEHAZING_HPP

/*
 * dehazing
 */
 
int dehazing(const IplImage *haze_img, IplImage *transmission, IplImage *dehaze_img, CvScalar A)
{
    CvScalar haze_pixel, t_pixel, dehaze_pixel;

    for (int i = 0; i < haze_img->height; i++) {
        for (int j = 0; j < haze_img->width; j++) {
            t_pixel = cvGet2D(transmission, i, j);      
            haze_pixel = cvGet2D(haze_img, i, j);

            // set a lower limit to avoid introduce noise
            if (t_pixel.val[0] / 255 < 0.1)
                t_pixel.val[0] = 0.1;
            else
                t_pixel.val[0] = t_pixel.val[0] / 255;

            for (int k = 0; k < haze_img->nChannels; k++) {
                dehaze_pixel.val[k] = (haze_pixel.val[k] - A.val[k]) / t_pixel.val[0] + A.val[k];
            }

            cvSet2D(dehaze_img, i, j, dehaze_pixel);
        }
    }

    return 1;
}

#endif
