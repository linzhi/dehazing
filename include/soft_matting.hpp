#ifndef SOFT_MATTING_HPP
#define SOFT_MATTING_HPP

#include <cmath>

double eps = pow(double(10), -8);
double lambda = pow(double(10), -4);

/*
 * soft matting to refine transmission image and I set the size of window is 3X3
 */

int laplacian_matrix(const Mat &src, int k_size)
{
    Mat dst = src.clone();
    
    if (k_size == 0) {
        dst = src.clone(); 
        return -1;
    }
    
    int k_center = (k_size - 1) / 2;
    
    for (int i = k_center; i < src.rows - k_center; i++) {
        for (int j = k_center; j < src.cols - k_center; j++) {
            Mat local_win = src.colRange(j-k_center, j+k_center+1).rowRange(i-k_center, i+k_center+1);

            Scalar mu = mean(local_win);

            Mat cov_matrix = Mat::zeros((Size(k_size, k_size)), CV_64FC1);
            
            for (int a = 0; a < local_win.rows; a++) {
                for (int b = 0; b < local_win.cols; b++) {
                    cov_matrix.at<double>(0, 0) += (local_win.at<Vec3b>(a, b)[0] - mu.val[0]) * (local_win.at<Vec3b>(a, b)[0] - mu.val[0]);
                    cov_matrix.at<double>(0, 1) += (local_win.at<Vec3b>(a, b)[0] - mu.val[0]) * (local_win.at<Vec3b>(a, b)[1] - mu.val[1]);
                    cov_matrix.at<double>(0, 2) += (local_win.at<Vec3b>(a, b)[0] - mu.val[0]) * (local_win.at<Vec3b>(a, b)[2] - mu.val[2]);
                    cov_matrix.at<double>(1, 1) += (local_win.at<Vec3b>(a, b)[1] - mu.val[1]) * (local_win.at<Vec3b>(a, b)[1] - mu.val[1]);
                    cov_matrix.at<double>(1, 2) += (local_win.at<Vec3b>(a, b)[1] - mu.val[1]) * (local_win.at<Vec3b>(a, b)[2] - mu.val[2]);
                    cov_matrix.at<double>(2, 2) += (local_win.at<Vec3b>(a, b)[2] - mu.val[2]) * (local_win.at<Vec3b>(a, b)[2] - mu.val[2]);
                }
            }
            
            cov_matrix.at<double>(0, 0) = (cov_matrix.at<double>(0, 0) + eps) / (k_size * k_size);
            cov_matrix.at<double>(0, 1) /= k_size * k_size;
            cov_matrix.at<double>(0, 2) /= k_size * k_size;
            cov_matrix.at<double>(1, 0) = cov_matrix.at<double>(0, 1);
            cov_matrix.at<double>(1, 1) = (cov_matrix.at<double>(1, 1) + eps) / (k_size * k_size);
            cov_matrix.at<double>(1, 2) /= k_size * k_size;
            cov_matrix.at<double>(2, 0) = cov_matrix.at<double>(0, 2);
            cov_matrix.at<double>(2, 1) = cov_matrix.at<double>(1, 2);
            cov_matrix.at<double>(2, 2) = (cov_matrix.at<double>(2, 2) + eps) / (k_size * k_size);
            
            Mat inv_cov_matrix = cov_matrix.inv(DECOMP_SVD);
        }
    }
}



#endif
