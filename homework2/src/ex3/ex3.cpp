#include <iostream>
#include <fstream>
#include <string>
#include <random>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

#define R 2*sqrt(2)
#define FRAME_CNT 300
#define SCALE 2
#define HEIGHT 800
#define WIDTH 800
#define PAUSE 100

std::vector<Eigen::Vector3d> read();

cv::VideoWriter init_writer();

std::vector<Eigen::Vector3d> calc(int frame, std::vector<Eigen::Vector3d> input, double &theta);

Eigen::Vector3d calc_camera_pos(int frame, double &theta);

void display(cv::Mat &drawer, std::vector<Eigen::Vector3d> points);

int get_random(int low, int high);


std::default_random_engine engine;


int main(int argc, char *argv[]) {
    std::vector<Eigen::Vector3d> input = read();
    cv::VideoWriter writer = init_writer();

    double theta = -M_PI * 3 / 20;
    cv::Mat drawer;
    for (int i = 0; i < FRAME_CNT && theta <= M_PI / 4; ++i) {
        drawer = cv::Mat::zeros({WIDTH, HEIGHT}, CV_8UC3);
        display(drawer, calc(i, input, theta));
        cv::imshow("result", drawer);
        cv::resizeWindow("result", {WIDTH, HEIGHT});
        writer << drawer;
        cv::waitKey(10);
    }

    // pause at end
    for (int j = 0; j <= PAUSE; ++j) {
        writer << drawer;
    }

    writer.release();
    return 0;
}


std::vector<Eigen::Vector3d> calc(int frame, std::vector<Eigen::Vector3d> input, double &theta) {
    std::vector<Eigen::Vector3d> res;
    res.resize(input.size());

    Eigen::Quaterniond q = {-0.5, 0.5, 0.5, -0.5};
    Eigen::Vector3d cam_w = calc_camera_pos(frame, theta);
    Eigen::Matrix4d converter = [&cam_w, &q]() {
        Eigen::Matrix4d converter = Eigen::Matrix4d::Zero();
        Eigen::Matrix3d rot_c_to_w = q.matrix();
        converter.block(0, 0, 3, 3) = rot_c_to_w.transpose().cast<double>();
        converter.block(0, 3, 3, 1) = -rot_c_to_w.transpose().cast<double>() * cam_w;
        converter(3,3)=1;
        return converter;
    }();

    for (int i = 0; i < res.size(); ++i) {
        Eigen::Vector4d w4;
        w4 << input[i](0, 0), input[i](1, 0), input[i](2, 0), 1.;
        Eigen::Matrix<double, 3, 4> cam_f;
        cam_f << 400., 0., 190., 0.,
                0., 400., 160., 0.,
                0., 0., 1., 0.;
        Eigen::Vector4d c4 = converter * w4;
        Eigen::Vector3d u3 = cam_f * c4;
        u3 /= u3(2, 0);
        res[i] = u3;
    }
    return res;
}

std::vector<Eigen::Vector3d> read() {
    std::ifstream ifs("../src/ex3/res/points.txt");
    int cnt;
    ifs >> cnt;
    std::vector<Eigen::Vector3d> input;
    for (int i = 0; i < cnt; ++i) {
        double x, y, z;
        ifs >> x >> y >> z;
        input.push_back({x, y, z});
    }
    ifs.close();
    return input;
}

Eigen::Vector3d calc_camera_pos(int frame, double &theta) {
//    std::cout << (2 - 2 * frame / (double) FRAME_CNT) << std::endl;
    theta += M_PI / (2.5 * FRAME_CNT) * (2 - 2 * frame / (double) FRAME_CNT);
//    std::cout << theta << std::endl;
    Eigen::Vector3d cam_w = {R * cos(theta), R * sin(theta), R * sin(theta)};
    return cam_w;
}

void display(cv::Mat &drawer, std::vector<Eigen::Vector3d> points) {
    for (int i = 0; i < points.size(); ++i) {
        int x = floor(points[i](0, 0) / SCALE)  + WIDTH / 2-250;
        int y = floor(points[i](1, 0) / SCALE) + HEIGHT / 2-200;
        //this condition is to reduce the points near the edge
        if ((pow(abs(x-WIDTH/2),2)+ pow(abs(y-HEIGHT/2),2)<= pow(WIDTH/4,2)|| !get_random(0,2))){
            cv::circle(drawer, {x, y}, get_random(1, 4),
                       {255, static_cast<double>(get_random(150, 255)),
                        static_cast<double>(get_random(150, 255))}, cv::FILLED);
        }
    }
}

int get_random(int low, int high) {
    std::uniform_int_distribution<int> dis(low, high);
    return dis(engine);
}

cv::VideoWriter init_writer() {
    std::string output_path = "../src/ex3/output/out.avi";
    double fps = 60;
    cv::Size size = cv::Size(WIDTH, HEIGHT);
    cv::VideoWriter writer(output_path,
                           cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                           fps, size, true);
    return writer;
}

