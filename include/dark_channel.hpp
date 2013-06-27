#ifndef DARK_CHANNEL_HPP
#define DARK_CHANNEL_HPP

#define WIN_SIZE  15

/*
 * get dark channel of hazy image
 */
 
static int get_dark_channel(const Mat &src, Mat &dark_channel)
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
    
    int win_center = (WIN_SIZE - 1) / 2;
    double min_value = 0;
    double max_value = 0;

    for (int i = win_center; i < dark_bgr.rows - win_center; i++) {
        for (int j = win_center; j < dark_bgr.cols - win_center; j++) {
            Mat patch = dark_bgr.colRange(j-win_center, j+win_center+1).rowRange(i-win_center, i+win_center+1);

            minMaxLoc(patch, &min_value, &max_value);

            for (int m = i-win_center; m < i+win_center+1; m++) {
                for (int n = j-win_center; n < j+win_center+1; n++) {
                    dark_channel.at<uchar>(m, n) = min_value;
                }
            }
        }
    }

    return 1;
}

#endif

