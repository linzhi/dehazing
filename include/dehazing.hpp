#ifndef DEHAZING_HPP
#define DEHAZING_HPP

/*
 * dehazing
 */
 
static int dehazing(const Mat &haze_img, const Mat &transmission, Mat &dehaze_img, Scalar A)
{
    Scalar t_pixel;
    Vec3b haze_pixel;

    for (int i = 0; i < haze_img.rows; i++) {
        for (int j = 0; j < haze_img.cols; j++) {
            t_pixel = transmission.at<uchar>(i, j);   
            haze_pixel = haze_img.at<Vec3b>(i, j);

            // set a lower limit to avoid introduce noise
            if (t_pixel.val[0] / 255 < 0.1)
                t_pixel.val[0] = 0.1;
            else
                t_pixel.val[0] = t_pixel.val[0] / 255;
            
            dehaze_img.at<Vec3b>(i, j)[0] = (haze_pixel.val[0] - A.val[0]) / t_pixel.val[0] + A.val[0];
            dehaze_img.at<Vec3b>(i, j)[1] = (haze_pixel.val[1] - A.val[1]) / t_pixel.val[0] + A.val[1];
            dehaze_img.at<Vec3b>(i, j)[2] = (haze_pixel.val[2] - A.val[2]) / t_pixel.val[0] + A.val[2];
        }
    }

    return 1;
}

#endif
