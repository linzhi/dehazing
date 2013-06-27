#ifndef SOFT_MATTING_HPP
#define SOFT_MATTING_HPP

#include <cmath>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/bindings/traits/ublas_matrix.hpp>
#include <boost/numeric/bindings/lapack/gesv.hpp>
#include <boost/numeric/bindings/traits/ublas_vector2.hpp>

namespace ublas = boost::numeric::ublas;
namespace lapack = boost::numeric::bindings::lapack;

double eps = pow(double(10), -8);
double lambda = pow(double(10), -4);

/*
 * soft matting to refine transmission image and I set the size of window is 3X3
 */

static int solve_laplacian_matrix(const Mat &src, const Mat &transmission, Mat &op_transmission, int k_size)
{
    ublas::matrix<double, ublas::column_major> laplacian_matrix(src.rows * src.cols, src.rows * src.cols);

    int k_center = (k_size - 1) / 2;
    
    // get laplacian matrix
    
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

            for (int a = 0; a < local_win.rows; a++) {
                for (int b = 0; b < local_win.cols; b++) {
                    int index_i = (i - k_center + a) * src.cols + j - k_center + b;
                    double diff_i[3] = {local_win.at<Vec3b>(a, b)[0] - mu.val[0],
                                        local_win.at<Vec3b>(a, b)[1] - mu.val[1],
                                        local_win.at<Vec3b>(a, b)[2] - mu.val[2]};

                    for (int c = 0; c < local_win.rows; c++) {
                        for (int d = 0; d < local_win.cols; d++) {
                            int index_j = (i - k_center + c) * src.cols + j - k_center + d;
                            double diff_j[3] = {local_win.at<Vec3b>(c, d)[0] - mu.val[0],
                                                local_win.at<Vec3b>(c, d)[1] - mu.val[1],
                                                local_win.at<Vec3b>(c, d)[2] - mu.val[2]};

                            double temp[3] = {inv_cov_matrix.at<double>(0, 0) * diff_j[0] +
                                              inv_cov_matrix.at<double>(0, 1) * diff_j[1] +
                                              inv_cov_matrix.at<double>(0, 2) * diff_j[2],
                                              inv_cov_matrix.at<double>(1, 0) * diff_j[0] +
                                              inv_cov_matrix.at<double>(1, 1) * diff_j[1] +
                                              inv_cov_matrix.at<double>(1, 2) * diff_j[2],
                                              inv_cov_matrix.at<double>(2, 0) * diff_j[0] +
                                              inv_cov_matrix.at<double>(2, 1) * diff_j[1] +
                                              inv_cov_matrix.at<double>(2, 2) * diff_j[2]};

                            double temp_value = diff_i[0] * temp[0] + diff_i[1] * temp[1] + diff_i[2] * temp[2];

                            if (index_i == index_j) {
                                laplacian_matrix(index_i, index_j) += 1.0 - (1 + temp_value) / (k_size * k_size);
                            }
                            else {
                                laplacian_matrix(index_i, index_j) += 0.0 - (1 + temp_value) / (k_size * k_size);
                            }
                        }
                    }
                }
            }
        }
    }

    // solve sparse linear systems for Ax = b

    ublas::vector<double> b(transmission.rows * transmission.cols);

    for (unsigned i = 0; i < laplacian_matrix.size1(); i++) {
        for (unsigned j = 0; j < laplacian_matrix.size2(); j++) {
            if (i == j) 
                laplacian_matrix(i, j) += lambda;
        }
    }

    for (int i = 0; i < transmission.rows; i++) {
        for (int j = 0; j < transmission.cols; j++) {
            b(i * transmission.cols + j) = lambda * transmission.at<uchar>(i, j);
        }
    }

    lapack::gesv(laplacian_matrix, b);

    for (int i = 0; i < op_transmission.rows; i++) {
        for (int j = 0; j < op_transmission.cols; j++) {
            op_transmission.at<uchar>(i, j) = b(i * op_transmission.cols + j);
        }
    }
}

#endif





