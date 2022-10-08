#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

bool judgeByArea(std::vector<cv::Point> &contour){
    return cv::contourArea(contour)>=1e3;
}

int main(){
    cv::Mat src=cv::imread("../src/ex2/res/origin.png");
    assert(src.channels()==3);
    // convert rgb=> cvt
    cv::Mat hsv;
    cv::cvtColor(src,hsv, cv::COLOR_RGB2HSV);
    // extract red & yellow
    cv::Mat hsv_result1,hsv_result2,hsv_result3;
    cv::inRange(hsv,cv::Scalar(95,150,130),cv::Scalar(124,255,255),hsv_result1);
    cv::inRange(hsv,cv::Scalar(125,43,46),cv::Scalar(155,255,255),hsv_result2);
    cv::inRange(hsv,cv::Scalar(90,200,90),cv::Scalar(120,255,160),hsv_result3);
    // combine
    cv::Mat ones_mat=cv::Mat::ones(cv::Size(src.cols,src.rows),CV_8UC1);
    cv::Mat grey= 255 * (ones_mat - (ones_mat - hsv_result1 / 255).mul(ones_mat - hsv_result2 / 255));
    grey= 255 * (ones_mat - (ones_mat - grey / 255).mul(ones_mat - hsv_result3 / 255));
    cv::imwrite("../src/ex2/output/grey.jpg",grey);
    //open operation
    cv::Mat open;
    cv::Mat element=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(4,4));
    cv::morphologyEx(grey, open, cv::MORPH_OPEN, element);
    cv::imwrite("../src/ex2/output/open.jpg", open);
    // find contour
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat drawer=cv::Mat::zeros(cv::Size(src.cols,src.rows),CV_8UC3);
    cv::Rect rect;
    cv::findContours(open,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_NONE);

    for (int i=0;i!=-1;i=hierarchy[i][0]) {
        if (judgeByArea(contours[i])){
            cv::drawContours(drawer,contours,i,{20,20,220},3);
            rect=cv::boundingRect(contours[i]);
        }
    }
    cv::imwrite("../src/ex2/output/contour.jpg",drawer);
    // draw rectangle
    cv::rectangle(src,{rect.x,rect.y},
                  {rect.x+rect.size().width,rect.y+rect.size().height},{20,220,20},3);
    cv::imshow("res",src);
    cv::imwrite("../src/ex2/output/output.jpg",src);
    cv::waitKey(0);
    return 0;
}
