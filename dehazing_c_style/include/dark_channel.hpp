#ifndef DARK_CHANNEL_HPP
#define DARK_CHANNEL_HPP

#define BLOCK_SIZE  15

/*
 * get dark channel of hazy image
 */
 
int get_dark_channel(const IplImage *src, IplImage *dark_channel)
{
    IplImage *dark_bgr = NULL;

    dark_bgr = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);

    uchar *dark_bgr_ptr = (uchar *)dark_bgr->imageData;
    uchar *dark_channel_ptr = (uchar *)dark_channel->imageData;

    memset(dark_channel_ptr, 255, dark_channel->width * dark_channel->height);

    for (int i = 0; i < src->height; i++) {
        for (int j = 0; j < src->width; j++) {
            CvScalar pixel;
            double temp;
            pixel = cvGet2D(src, i, j);
            if (pixel.val[0] < pixel.val[1])
                temp = pixel.val[0];
            else
                temp = pixel.val[1];
            if (temp > pixel.val[2])
                temp = pixel.val[2];

            dark_bgr_ptr[i*dark_bgr->widthStep + j] = temp;
        }
    }

    int patch = BLOCK_SIZE / 2;
    int left_top_x = 0;
    int left_top_y = 0;
    int right_buttom_x = 0;
    int right_buttom_y = 0;
    
    for(int i = 0; i < dark_bgr->height; i++) {
        for(int j = 0; j < dark_bgr->width; j++) {
            left_top_x = j - patch > 0 ? j - patch : 0;
            left_top_y = i - patch > 0 ? i - patch : 0;
            right_buttom_x = j + patch < dark_bgr->width - 1 ? j + patch : dark_bgr->width - 1;
            right_buttom_y = i + patch < dark_bgr->height - 1 ? i + patch : dark_bgr->height - 1;
            
            for(int m = left_top_y; m <= right_buttom_y; m++) {
                for(int n = left_top_x; n <= right_buttom_x; n++) {
                    if(dark_bgr_ptr[i*dark_bgr->widthStep + j] < dark_channel_ptr[m*dark_bgr->widthStep + n])
                        dark_channel_ptr[m*dark_bgr->widthStep + n] = dark_bgr_ptr[i*dark_bgr->widthStep + j];
                }
            }
        }
    }

    cvReleaseImage(&dark_bgr);

    return 1;
}

#endif

