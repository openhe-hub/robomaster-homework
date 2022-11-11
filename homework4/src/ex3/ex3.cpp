#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(){
    // 1. init & read
    cv::VideoCapture left(R"(../src/ex3/res/left_10.mp4)");
    cv::VideoCapture right(R"(../src/ex3/res/right_10.mp4)");
    cv::FileStorage extrinsics(R"(../src/ex3/res/extrinsics.yml)",cv::FileStorage::READ);
    cv::FileStorage stereo(R"(../src/ex3/res/stereo.yaml)",cv::FileStorage::READ);

    cv::Mat src_left,src_right;
    left>>src_left;
    right>>src_right;
    cv::Size size={src_right.cols,src_right.rows};

    cv::Mat R,T,R1,R2,P1,P2,K1,K2,C1,C2,Q;
    extrinsics["R"]>>R;
    extrinsics["T"]>>T;
    extrinsics["R1"]>>R1;
    extrinsics["R2"]>>R2;
    extrinsics["P1"]>>P1;
    extrinsics["P2"]>>P2;
    stereo["K_0"]>>K1; // 内参
    stereo["K_1"]>>K2;
    stereo["C_0"]>>C1; // 畸变矩阵
    stereo["C_1"]>>C2;

    // 2. 立体矫正
    cv::Rect roi1,roi2;
    cv::stereoRectify(K1,C1,K2,C2,size,R,T,R1,R2,P1,P2,Q,
                      cv::CALIB_ZERO_DISPARITY,-1,size,&roi1,&roi2);

    // 3. 矫正映射
    cv::Mat map11,map12,map21,map22;
    cv::initUndistortRectifyMap(K1,C1,R1,P1,size,CV_16SC2,map11,map12);
    cv::initUndistortRectifyMap(K2,C2,R2,P2,size,CV_16SC2,map21,map22);

    cv::Mat img1_undistort,img2_undistort;
    cv::remap(src_left,img1_undistort,map11,map12,cv::INTER_LINEAR);
    cv::remap(src_right,img2_undistort,map21,map22,cv::INTER_LINEAR);

//    cv::imshow("src-left",src_left);
//    cv::imshow("src-right",src_right);
//    cv::imshow("undistorted left",img1_undistort);
//    cv::imshow("undistorted right",img2_undistort);

    // 4. BM 立体匹配算法
    cv::Ptr<cv::StereoBM> bm=cv::StereoBM::create(15*16,19);
    bm->setROI1(roi1);
    bm->setROI2(roi2);
//    bm->setUniquenessRatio(10);


//    bm->setPreFilterCap(13);
//    bm->setMinDisparity(0);
//
//    bm->setTextureThreshold(20);
//
//    bm->setSpeckleWindowSize(13);
//    bm->setSpeckleRange(32);
//    bm->setDisp12MaxDiff(1);

    cv::Mat res,left_gray,right_gray;
    cv::cvtColor(img1_undistort,left_gray,cv::COLOR_RGB2GRAY);
    cv::cvtColor(img2_undistort,right_gray,cv::COLOR_RGB2GRAY);
    bm->compute(left_gray,right_gray,res);
    cv::imshow("output",res);

    cv::waitKey(0);
    return 0;
}

