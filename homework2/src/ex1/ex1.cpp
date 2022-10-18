#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main() {
    cv::Mat src = cv::imread("../src/ex1/res/origin.png");
    // rgb => hsv
    cv::Mat hsv;
    cv::cvtColor(src, hsv, cv::COLOR_RGB2HSV);
    // hsv extraction
    cv::Mat hsv_result;
    cv::inRange(hsv, cv::Scalar(0, 43, 46), cv::Scalar(10, 255, 255), hsv_result);
    // close operation
    cv::Mat close;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::morphologyEx(hsv_result, close, cv::MORPH_CLOSE, element);
    cv::imwrite("../src/ex1/output/hsv.jpg",close);
    // find contour
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat drawer = cv::Mat::zeros(cv::Size(src.cols, src.rows), CV_8UC3);
    cv::findContours(close, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
    int max_idx = -1;
    double max_val = -1;
    for (int i = 0; i != -1; i = hierarchy[i][0]) {
        if (cv::contourArea(contours[i]) > max_val) {
            max_idx = i;
            max_val = cv::contourArea(contours[i]);
        }
    }
    cv::drawContours(drawer, contours, max_idx, {20, 20, 220}, 3);
    cv::imwrite("../src/ex1/output/contour.jpg",drawer);

    // get bounding rectangle
    cv::Rect rect = cv::boundingRect(contours[max_idx]);

    // transformation
    cv::Point2f srcPts[4], dstPts[4];
    cv::Mat result = cv::Mat::zeros(rect.size(), CV_8UC3);
    const int dx=12;
    srcPts[0] = {static_cast<float>(rect.x-dx), static_cast<float>(rect.y)};
    srcPts[1] = {static_cast<float>(rect.x + rect.size().width), static_cast<float>(rect.y)};
    srcPts[2] = {static_cast<float>(rect.x), static_cast<float>(rect.y + rect.size().height)};
    srcPts[3] = {static_cast<float>(rect.x + rect.size().width+dx), static_cast<float>(rect.y + rect.size().height)};

    dstPts[0] = {static_cast<float>(0), static_cast<float>(0)};
    dstPts[1] = {static_cast<float>(rect.size().width), static_cast<float>(0)};
    dstPts[2] = {static_cast<float>(0), static_cast<float>(rect.size().height)};
    dstPts[3] = {static_cast<float >(rect.size().width), static_cast<float >(rect.size().height)};

    cv::Mat transform = cv::getPerspectiveTransform(srcPts, dstPts);
    cv::warpPerspective(src, result, transform, src.size());
    result = result(cv::Rect(0, 0, rect.size().width, rect.size().height));
    cv::imshow("result", result);
    cv::imwrite("../src/ex1/output/result.jpg", result);

    //draw rectangle
    cv::rectangle(src, {rect.x, rect.y},
                  {rect.x + rect.size().width, rect.y + rect.size().height}, {20, 220, 20}, 3);
    cv::imwrite("../src/ex1/output/rect.jpg",src);

    cv::waitKey(0);
    return 0;
}


