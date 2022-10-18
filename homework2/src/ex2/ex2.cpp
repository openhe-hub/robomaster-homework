#include <iostream>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

int main() {
    std::ifstream ifs("../src/ex2/res/points.txt");
    std::ofstream ofs("../src/ex2/output/out.txt");
    int num_points = 0;
    ifs >> num_points;

    Eigen::Vector3d cam_w = {2., 2., 2.};
    Eigen::Quaterniond q = {0.5, 0.5, -0.5, -0.5};
    Eigen::Matrix4d converter = [&cam_w, &q]() {
        Eigen::Matrix4d converter = Eigen::Matrix4d::Zero();
        Eigen::Matrix3d rot_c_to_w = q.matrix();
        converter.block(0, 0, 3, 3) = rot_c_to_w.transpose().cast<double>();
        converter.block(0, 3, 3, 1) = -rot_c_to_w.transpose().cast<double>() * cam_w;
        return converter;
    }();

    for (int i = 0; i < num_points; ++i) {
        double x, y, z;
        ifs >> x >> y >> z;
        Eigen::Vector4d w4;
        w4 << x, y, z, 1.;
        Eigen::Matrix<double,3,4> cam_f;
        cam_f<<400., 0., 190., 0.,
                0., 400., 160., 0.,
                0., 0., 1., 0.;
        Eigen::Vector4d c4=converter*w4;
        Eigen::Vector3d u3=cam_f*c4;
        u3/=u3(2,0);
        ofs<<u3(0,0)<<","<<u3(1,0)<<"\n";
    }
    return 0;
}

