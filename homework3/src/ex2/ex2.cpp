#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Eigen/Dense>

#define N  100
#define PREDICTION 10


int main() {
    //get data
    std::ifstream ifs(R"(../src/ex2/res/dollar.txt)");
    std::ofstream ofs(R"(../src/ex2/out/res.txt)");
    double data[N],buffer;
    int cnt=0;

    while (ifs>>buffer) data[cnt++]=buffer;

    std::cout<<"origin data:\n";
    for (int i = 0; i < cnt; ++i) {
        std::cout<<data[i]<<(i==cnt-1?"\n":"\t");
    }


    const int Z_N=1,X_N=2;
    Eigen::Matrix<double,X_N,1> X;   //状态数据
    Eigen::Matrix<double,X_N,X_N> A; //状态转移矩阵
    Eigen::Matrix<double,X_N,X_N> P; //预测噪声协方差
    Eigen::Matrix<double,X_N,X_N> R; //预测过程噪声偏差的协方差
    Eigen::Matrix<double,X_N,Z_N> K; //卡尔曼增益
    Eigen::Matrix<double,Z_N,X_N> C; //观测矩阵
    Eigen::Matrix<double,Z_N,Z_N> Q; //测量噪声协方差

    X<<data[0],0;
    A<<1,1,0,1;
    C<<1,0;
    R<<2,0,0,2;
    Q<<10;

    for (int i = 1; i < cnt+PREDICTION; ++i) {
        //update prediction
        Eigen::Matrix<double,X_N,1> X_k=A*X;
        P=A*P*A.transpose()+R;
        //update observation
        K=P*C.transpose()*(C*P*C.transpose()+Q).inverse();
        Eigen::Matrix<double,Z_N,1> Z{i<cnt?data[i]:X(0)+X_k(1)};
        X=X_k+K*(Z-C*X_k);
        P=(Eigen::Matrix<double,X_N,X_N>::Identity()-K*C)*P;
        std::cout<<"step "<<i<<":"<<X(0)<<std::endl;
        ofs<<X(0)<<std::endl;
    }
    std::cout<<"final prediction="<<X(0)<<std::endl;
}
