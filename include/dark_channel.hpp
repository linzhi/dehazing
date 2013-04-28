#ifndef DARK_CHANNEL_HPP
#define DARK_CHANNEL_HPP

#define BLOCK_SIZE  15

/*
 * get dark channel of hazy image
 */
 
int get_dark_channel(const Mat &src, Mat &dark_channel)
{
    Mat dark_bgr = Mat::zeros(src.size(), CV_8UC1);
    dark_channel = Scalar::all(255);

    for (int i = 0; i < src.rows; i++) {
        for (int j = 0; j < src.cols; j++) {
            uchar temp;
            
            if (src.at<Vec3b>(i, j)[0] < src.at<Vec3b>(i, j)[1])
                temp = src.at<Vec3b>(i, j)[0];
            else
                temp = src.at<Vec3b>(i, j)[1];
                
            if (temp > src.at<Vec3b>(i, j)[2])
                temp = src.at<Vec3b>(i, j)[2];

            dark_bgr.at<uchar>(i, j) = temp;
        }
    }
    
    int patch = BLOCK_SIZE / 2;
    int left_top_x = 0;
    int left_top_y = 0;
    int right_buttom_x = 0;
    int right_buttom_y = 0;
    
    for(int i = 0; i < dark_bgr.rows; i++) {
        for(int j = 0; j < dark_bgr.cols; j++) {
            left_top_x = j - patch > 0 ? j - patch : 0;
            left_top_y = i - patch > 0 ? i - patch : 0;
            right_buttom_x = j + patch < dark_bgr.cols - 1 ? j + patch : dark_bgr.cols - 1;
            right_buttom_y = i + patch < dark_bgr.rows - 1 ? i + patch : dark_bgr.rows - 1;
            
            for(int m = left_top_y; m <= right_buttom_y; m++) {
                for(int n = left_top_x; n <= right_buttom_x; n++) {
                    if(dark_bgr.at<uchar>(i, j) < dark_channel.at<uchar>(m, n))
                        dark_channel.at<uchar>(m, n) = dark_bgr.at<uchar>(i, j);
                }
            }
        }
    }

    return 1;
}

#endif

