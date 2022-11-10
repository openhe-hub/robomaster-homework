#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(){
    // 1. rgb => gray
    cv::Mat img1=cv::imread(R"(../src/ex1/res/img1.png)");
    cv::Mat img2=cv::imread(R"(../src/ex1/res/img2.png)");
    cv::Mat gray1,gray2;
    cv::cvtColor(img1,gray1,cv::COLOR_RGB2GRAY);
    cv::cvtColor(img2,gray2,cv::COLOR_RGB2GRAY);
    // 2. orb: detect feature points
    cv::Ptr<cv::ORB> orb=cv::ORB::create();
    std::vector<cv::KeyPoint> key_points1;
    std::vector<cv::KeyPoint> key_points2;
    orb->detect(gray1,key_points1);
    orb->detect(gray2,key_points2);
    // 3. orb: compute descriptor
    cv::Mat descriptor1,descriptor2;
    orb->compute(gray1,key_points1,descriptor1);
    orb->compute(gray2,key_points2,descriptor2);
    // 4. match: Hamming distance match O(n^2)
    cv::BFMatcher matcher;
    std::vector<cv::DMatch> pairs;
    matcher.match(descriptor1,descriptor2,pairs);
    for (auto & pair : pairs) {
        std::cout<<pair.queryIdx<<","<<pair.trainIdx
        <<",dis="<<pair.distance<<"\n";
    }
    // 5. filter
    std::vector<cv::DMatch> good_pairs;
    float min_dis=1e5;
    for (auto & pair : pairs) {
        min_dis=min_dis>pair.distance?pair.distance:min_dis;
    }
    for (auto & pair : pairs) {
        if (pair.distance<=2.5*min_dis){
            good_pairs.push_back(pair);
        }
    }
    // 6. draw
    cv::Mat drawer;
    cv::drawMatches(img1,key_points1,img2,key_points2,good_pairs,drawer);
//    cv::imshow("result",drawer);
//    cv::resizeWindow("result",600,800);
    cv::imwrite(R"(../src/ex1/out/out.jpg)",drawer);

    cv::drawKeypoints(img1,key_points1,img1,{25,225,25},cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::imwrite(R"(../src/ex1/out/key_points.jpg)",img1);
//    cv::waitKey(0);
    return 0;
}

