#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define N 20


int main() {
    // init size & path
    const int board_width = 11, board_height = 8;
    const int board_n = board_width * board_height;
    cv::Size board_size(board_width, board_height);
    std::string path = R"(../src/ex4/res/)";
    std::string prefix = ".jpg";

    cv::Mat gray_img, drawer;
    std::vector<cv::Point2f> point_pix_pos_tmp;
    std::vector<std::vector<cv::Point2f>> point_pix_pos;

    int successes = 0;
    cv::Size img_size;

    for (int i = 0; i < N; ++i) {
        cv::Mat src = cv::imread(path + std::to_string(i) + prefix);
        if (!i) img_size = {src.cols, src.rows};
        bool is_found = cv::findChessboardCorners(src, board_size, point_pix_pos_tmp);
        if (is_found && point_pix_pos_tmp.size() == board_n) {
            successes++;
            // find 4 quad corner sub-pixels
            cv::cvtColor(src, gray_img, cv::COLOR_BGR2GRAY);
            cv::find4QuadCornerSubpix(gray_img, point_pix_pos_tmp, cv::Size(5, 5));
            point_pix_pos.push_back(point_pix_pos_tmp);
            drawer = src.clone();
            cv::drawChessboardCorners(drawer, board_size, point_pix_pos_tmp, is_found);
            cv::imshow("corners", drawer);
            cv::waitKey(100);
        } else {
            //not found
            std::cout << "error at picture " << i << std::endl;
        }
        point_pix_pos_tmp.clear();
    }
    std::cout << successes << " useful pictures" << std::endl;

    // set board square size
    cv::Size square_size(10.0, 10.0);
    std::vector<std::vector<cv::Point3f>> point_grid_pos;
    std::vector<cv::Point3f> point_grid_pos_tmp;
    std::vector<int> point_cnt;

    cv::Mat camera_mat(3, 3, CV_32FC1, cv::Scalar::all(0));
    cv::Mat distort_mat(1, 5, CV_32FC1, cv::Scalar::all(0));
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;

    for (int i = 0; i < successes; ++i) {
        for (int j = 0; j < board_height; ++j) {
            for (int k = 0; k < board_width; ++k) {
                cv::Point3f pt = {(float) k * square_size.width, (float) j * square_size.height, 0};
                point_grid_pos_tmp.push_back(pt);
            }
        }
        point_grid_pos.push_back(point_grid_pos_tmp);
        point_grid_pos_tmp.clear();
        point_cnt.push_back(board_n);
    }
    // return rms re-projection err
    double rms = cv::calibrateCamera(point_grid_pos, point_pix_pos, img_size, camera_mat, distort_mat, rvecs, tvecs);
    std::cout << rms << std::endl;
    std::cout << camera_mat << std::endl << distort_mat << std::endl;
    // write to res2.txt
    std::ofstream ofs(R"(../src/ex4/out/res2.txt)");
    ofs << "rms re-projection error= " << rms << std::endl;
    ofs << "camera internal params=\n" << camera_mat << std::endl;
    ofs << "distort matrix=" << distort_mat << std::endl;
    return 0;
}

