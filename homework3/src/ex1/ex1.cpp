#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>


std::vector<cv::Point2f> pts_origin = {{575.508, 282.175},
                                       {573.93,  331.819},
                                       {764.518, 337.652},
                                       {765.729, 286.741}};
std::vector<cv::Point3f> pts_dst = {{-0.115, 0.0265,  0.},
                                    {-0.115, -0.0265, 0.},
                                    {0.115,  -0.0265, 0.},
                                    {0.115,  0.0265,  0.}};


cv::Mat camera_mat = (cv::Mat_<float>(3, 3) << 1.4142055480888289e+03, 0., 6.5792667877101735e+02
        , 0., 1.4137212634286254e+03, 4.0455967287503142e+02
        , 0.0, 0.0, 1.0);
cv::Mat distort_mat = (cv::Mat_<float>(1, 5)
        << -1.5713532786218060e-01, 3.3883933349759310e-01, 7.0103028435554145e-04, -2.9035486504482461e-04, -4.2372708010388316e-01);


void locate(cv::Mat &drawer);

Eigen::Vector4d rotate(cv::Mat tvec);

int main() {
    // init and locate the armor points
    cv::Mat src = cv::imread(R"(../src/ex1/res/hero.jpg)");
    locate(src);
    cv::imshow("locate armor", src);
    cv::waitKey(1000);
    // solve pnp
    cv::Mat rvec, tvec;
    cv::solvePnP(pts_dst, pts_origin, camera_mat, distort_mat, rvec, tvec);
    std::cout << "t:" << std::endl << -tvec << std::endl;
    // rotate: from cam to imu
    Eigen::Vector4d ret= rotate(tvec);
    std::cout<<ret<<std::endl;
    std::ofstream ofs(R"(../src/ex1/out/res.txt)");
    ofs<<"("<<ret(0)<<","<<ret(1)<<","<<ret(2)<<")"<<std::endl;
    return 0;
}

void locate(cv::Mat &drawer) {
    for (int i = 0; i < 4; ++i) {
        cv::circle(drawer, pts_origin[i], 5, {255, 25, 225});
    }
}

// camera => imu
Eigen::Vector4d rotate(cv::Mat tvec) {
    //d,a,b,c => ai+bj+ck+d
    // 0.994363i + -0.0676645j + -0.00122528k + -0.0816168
    Eigen::Quaterniond q = {-0.0816168, 0.994363, -0.0676645, -0.00122528};
    // set camera as origin point
    Eigen::Vector3d cam_w = {0,0,0};
    Eigen::Matrix4d converter = [&cam_w, &q]() {
        Eigen::Matrix4d converter = Eigen::Matrix4d::Zero();
        Eigen::Matrix3d rot_c_to_w = q.matrix();
        // camera => world
        converter.block(0, 0, 3, 3) = rot_c_to_w.cast<double>();
        converter.block(0, 3, 3, 1) = -rot_c_to_w.cast<double>() * cam_w;
        converter(3,3)=1;
        return converter;
    }();

    Eigen::Vector4d pt_cam;
    pt_cam<<-tvec.at<double>(0,0),-tvec.at<double>(1,0),-tvec.at<double>(2,0),1;
    Eigen::Vector4d  ret=converter*pt_cam;
    return ret;
}
