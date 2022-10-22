#include <iostream>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>

#include <GL/glut.h>

#define N 80
#define R 0.005f
#define SCALE 1200

std::vector<Eigen::Vector3d> res;

void calc();

void display();

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("result");

    calc();
    glutDisplayFunc(&display);

    glutMainLoop();
    return 0;
}

void calc() {
    std::ifstream ifs("../src/ex2/res/points.txt");
    std::ofstream ofs("../src/ex2/output/out.txt");
    int num_points = 0;
    ifs >> num_points;

    Eigen::Vector3d cam_w = {2., 2., 2.};
    //陀螺仪给出的四元数，代表相机坐标系转世界坐标系
    Eigen::Quaterniond q = {-0.5, 0.5, 0.5, -0.5};  //d,a,b,c => ai+bj+ck+d
    Eigen::Matrix4d converter = [&cam_w, &q]() {
        Eigen::Matrix4d converter = Eigen::Matrix4d::Zero();
        Eigen::Matrix3d rot_c_to_w = q.matrix();
        //旋转矩阵是正交矩阵，所以矩阵的转置就是矩阵的逆，代表世界坐标系转相机坐标系
        converter.block(0, 0, 3, 3) = rot_c_to_w.transpose().cast<double>();
        //相机在相机坐标系的原点，所以取负
        converter.block(0, 3, 3, 1) = -rot_c_to_w.transpose().cast<double>() * cam_w;
        return converter;
    }();

    for (int i = 0; i < num_points; ++i) {
        double x, y, z;
        ifs >> x >> y >> z;
        Eigen::Vector4d w4;
        w4 << x, y, z, 1.;
        Eigen::Matrix<double, 3, 4> cam_f;
        //相机内参矩阵
        cam_f << 400., 0., 190., 0.,
                0., 400., 160., 0.,
                0., 0., 1., 0.;
        Eigen::Vector4d c4 = converter * w4;
        Eigen::Vector3d u3 = cam_f * c4;
        //归一化至二维像素坐标系，即z=1的平面点
        u3 /= u3(2, 0);
        res.push_back(u3);
        ofs << u3(0, 0) << " " << u3(1, 0) << "\n";
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    for (Eigen::Vector3d pt:res) {
        double x=pt(0,0), y=pt(1,0);
        glBegin(GL_POLYGON);
        for (int j = 0; j < N; ++j) {
            glVertex2d(x / SCALE + R * cos(2 * M_PI / N * j)-0.5,
                       -y / SCALE + R * sin(2 * M_PI / N * j)+0.3);
        }
        glEnd();
    }
    glFlush();
}

