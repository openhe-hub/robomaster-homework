#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "VO.h"


int main() {
    // 0. read camera internal param
    cv::FileStorage camera_params(R"(../src/ex2/res/camera.yaml)", cv::FileStorage::READ);
    cv::Mat img1 = cv::imread(R"(../src/ex2/res/0_orig.jpg)");
    cv::Mat img2 = cv::imread(R"(../src/ex2/res/1_orig.jpg)");


    cv::Mat K = camera_params["K"].mat();
    // 1. match & pnp
    cv::Mat depth1 = cv::imread(R"(../src/ex2/res/0_dpt.tif)",cv::IMREAD_LOAD_GDAL|cv::IMREAD_ANYDEPTH);
    cv::Mat depth2 = cv::imread(R"(../src/ex2/res/1_dpt.tif)",cv::IMREAD_LOAD_GDAL|cv::IMREAD_ANYDEPTH);
    cv::Mat rvec,tvec;
    find_PnP(img1,depth1,img2,depth2,K,rvec,tvec);
    // 2. concat picture
    process_Stitch_project(img1,depth1,img2,depth2,K,rvec,tvec,R"(../src/ex2/out/res.jpg)");
    return 0;
}
